
#include "yack/chem/outcome.hpp"

namespace yack
{
    namespace chemical
    {
        outcome:: outcome(const components::state s,
                          const extent::grade     g,
                          const double            x) throw() :
        state(s),
        grade(g),
        value(x)
        {

        }

        outcome:: ~outcome() throw()
        {
        }

        outcome:: outcome(const outcome &other) throw() :
        state(other.state),
        grade(other.grade),
        value(other.value)
        {
        }
        
    }

}

#include "yack/math/iota.hpp"
#include "yack/math/triplet.hpp"
#include "yack/system/imported.hpp"
#include "yack/ios/ocstream.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        struct MassActionF
        {
            const components       &c;
            const double            K;
            const readable<double> &C;
            rmulops                &m;
            size_t                  n;

            inline double operator()(const double x)
            {
                ++n;
                return c.mass_action(K,C,x,m);
            }
        };




        enum search_extent
        {
            search_positive_extent,
            search_negative_extent
        };

        outcome outcome:: study(const components       &comp,
                                const double            K,
                                const readable<double> &Cini,
                                writable<double>       &Cend,
                                rmulops                &xmul,
                                raddops                &xadd)
        {

            static const char fn[] = "chemical::outcome::study";
            assert(K>0);
            assert(comp.size()>0);

            //------------------------------------------------------------------
            //
            // initialize Cend=Cini and check state
            //
            //------------------------------------------------------------------
            iota::save(Cend,Cini);
            switch( comp.state_at(Cini) )
            {
                case components::are_blocked: return outcome(components::are_blocked,extent::is_degenerated,0);
                case components::are_running: break;
            }

            //------------------------------------------------------------------
            //
            // create function from Cend
            //
            //------------------------------------------------------------------
            MassActionF      F  = { comp, K, Cend, xmul, 0};
            double           AX = -1;

        FIND_XI:
            triplet<double>  x  = { 0,0,0 };                // initialize x
            triplet<double>  f  = { 0,F(0),0 };             // initialize f
            const sign_type  s  = __sign::of(f.b);          // check initial sign
            search_extent    d  = search_positive_extent;   // to be set w.r.t sign
            switch(s)
            {
                case __zero__:  goto SUCCESS;                     // early return
                case positive: d = search_positive_extent; break; // xi>0
                case negative: d = search_negative_extent; break; // xi<0
            }


            //------------------------------------------------------------------
            //
            // bracket solution
            //
            //------------------------------------------------------------------
            {
                const xlimits &       xlms = comp.genuine_limits(Cend,0);
                const xlimit  * const rlim = xlms.reac;
                const xlimit  * const plim = xlms.prod;
                //std::cerr << "limits=" << xlms << std::endl;

                switch(xlms.type)
                {

                    case limited_by_none:
                        throw imported::exception(fn,"unexpected unlimited");

                    case limited_by_both:
                        switch(d)
                        {
                            case search_positive_extent:
                                x.a = x.b;      assert(fabs(x.a)<=0);
                                f.a = f.b;      assert(f.a>0);
                                x.c = rlim->xi; assert(x.c>0);
                                f.c = F(x.c);   assert(f.c<0);
                                break;

                            case search_negative_extent:
                                x.c = x.b; assert(fabs(x.c)<=0);
                                f.c = f.b; assert(f.c<0);
                                x.a = -(plim->xi);
                                f.a = F(x.a); assert(f.a>0);
                                break;
                        }
                        break;

                    case limited_by_reac:
                        assert(comp.prod.molecularity==0);
                        switch (d)
                        {
                            case search_negative_extent:
                                x.c = x.b; assert(fabs(x.c)<=0);
                                f.c = f.b; assert(f.c<0);
                                x.a = -pow(K,-1.0/comp.reac.molecularity);
                                while( (f.a = F(x.a)) <=0 ) x.a += x.a;
                                assert(x.a<0);
                                assert(f.a>0);
                                break;

                            case search_positive_extent:
                                x.a = x.b;      assert(fabs(x.a)<=0);
                                f.a = f.b;      assert(f.a>0);
                                x.c = rlim->xi; assert(x.c>0);
                                f.c = -1;
                                break;
                        }

                        break;

                    case limited_by_prod:
                        assert(comp.reac.molecularity==0);
                        switch (d)
                        {
                            case search_negative_extent:
                                x.c = x.b;          assert(fabs(x.c)<=0);
                                f.c = f.b;          assert(f.c<0);
                                x.a = -(plim->xi);  assert(x.a<0);
                                f.a = K;            assert(f.a>0);
                                break;

                            case search_positive_extent:
                                x.a = x.b; assert( fabs(x.a) <= 0);
                                f.a = f.b; assert(f.a>0);
                                x.c = pow(K,1.0/comp.prod.molecularity);
                                while( (f.c = F(x.c)) >=0 ) x.c += x.c;
                                assert(x.c>0);
                                assert(f.c<0);
                                break;
                        }
                        break;

                }
            }

            //std::cerr << "search: x=" << x << ", f=" << f << std::endl;
            assert( __sign::product_of(f.a,f.c) == negative );


            //------------------------------------------------------------------
            //
            // inner loop
            //
            //------------------------------------------------------------------
            {
                double width = fabs(x.c-x.a);

            LOOP:
                assert(f.a>0);
                assert(f.c<0);
                assert(x.a<x.c);
                f.b = F( x.b = clamp(x.a,0.5*(x.a+x.c),x.c) ); assert(x.is_increasing());

                switch( __sign::of(f.b) )
                {
                    case __zero__:
                        comp.move(Cend,x.b);
                        goto SUCCESS;

                    case negative:
                        f.c = f.b;
                        x.c = x.b;
                        break;

                    case positive:
                        f.a = f.b;
                        x.a = x.b;
                        break;
                }

                const double new_width = fabs(x.c-x.a);
                if(new_width<=0 || new_width>=width)
                {
                    const double xx = x.b;
                    const double ax = fabs(xx);
                    comp.move(Cend,x.b);
                    if(AX>0 && ax>=AX) goto SUCCESS;
                    AX = ax;
                    goto FIND_XI;
                }
                width = new_width;
                goto LOOP;
            }




        SUCCESS:
            const double xi = comp.estimate_extent(Cini,Cend,xadd);
            std::cerr << "F(" << xi << ")=" << comp.mass_action(K,Cend,xmul) << std::endl;
            return outcome(components::are_running,comp.qualify_extent(xi,Cini,xmul),xi);
        }

    }

}




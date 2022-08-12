
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
            const components       &comp;
            const double            K;
            const readable<double> &C;
            rmulops                &mul;

            double operator()(const double xx)
            {
                return comp.mass_action(K,C,xx,mul);
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

            assert(K>0);

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            iota::save(Cend,Cini);
            switch( comp.state_at(Cini) )
            {
                case components::are_blocked: return outcome(components::are_blocked,extent::is_degenerated,0);
                case components::are_running: break;
            }

            MassActionF      F  = { comp, K, Cend, xmul };
            triplet<double>  f  = { 0,F(0),0 };             // initalize f
            const sign_type  s  = __sign::of(f.b);          // check sign
            search_extent    d  = search_positive_extent;   // to be set
            switch(s)
            {
                case __zero__: return outcome(components::are_running,extent::is_degenerated,0); // early return
                case positive: d = search_positive_extent; break;                                // xi>0
                case negative: d = search_negative_extent; break;                                // xi<0
            }
            triplet<double>  x  = { 0,0,0 };


            //------------------------------------------------------------------
            //
            // bracket solution
            //
            //------------------------------------------------------------------
            {
                const xlimits &       xlms = comp.genuine_limits(Cini,0);
                const xlimit  * const rlim = xlms.reac;
                const xlimit  * const plim = xlms.prod;
                std::cerr << "limits=" << xlms << std::endl;

                switch(xlms.type)
                {

                    case limited_by_none:
                        return outcome(components::are_running,extent::is_degenerated,0);

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

            std::cerr << "x=" << x << ", f=" << f << std::endl;
            assert( __sign::product_of(f.a,f.c) == negative );


            //------------------------------------------------------------------
            //
            // loop
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
                    case __zero__: goto SUCCESS;
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
                if(new_width<=0||new_width>=width)
                {
                    goto SUCCESS;
                }
                width = new_width;
                goto LOOP;
            }

        SUCCESS:
            comp.move(Cend,x.b);

            const double xi = comp.estimate_extent(Cini,Cend,xadd);
            std::cerr << "success@" << xi << std::endl;
            return outcome(components::are_running,comp.qualify_extent(xi,Cini,xmul),xi);
        }

    }

}




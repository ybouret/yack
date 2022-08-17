
#include "yack/chem/outcome.hpp"
#include <iomanip>

namespace yack
{
    namespace chemical
    {

        outcome:: outcome() throw() :
        state(components::are_blocked),
        grade(extent::is_degenerated),
        value(0)
        {
        }


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

        outcome & outcome:: operator=(const outcome &other) throw()
        {
            coerce(state) = other.state;
            coerce(grade) = other.grade;
            coerce(value) = other.value;
            return *this;
        }


        std::ostream & operator<<(std::ostream &os, const outcome &out)
        {
            os << components::state_text(out.state);
            switch(out.grade)
            {
                case extent::is_degenerated: os << " [0] "; break;
                case extent::is_significant: os << " [*] "; break;
            }
            os << std::setw(15) << out.value;
            return os;
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
                                raddops                &xadd,
                                size_t                 *call)
        {

            static const char fn[] = "chemical::outcome::study";
            assert(K>0);
            assert(comp.size()>0);

            //------------------------------------------------------------------
            //
            // initialize Cend=Cini
            //
            //------------------------------------------------------------------
            iota::save(Cend,Cini);

            //------------------------------------------------------------------
            //
            // check if components are running
            //
            //------------------------------------------------------------------
            switch( comp.state_at(Cini) )
            {
                case components::are_blocked: return outcome(components::are_blocked,extent::is_degenerated,0); // early return
                case components::are_running: break;
            }

            //------------------------------------------------------------------
            //
            // create function from Cend
            //
            //------------------------------------------------------------------
            MassActionF      F  = { comp, K, Cend, xmul, 0 };
            double           AX = -1; // absolute Xi, must decrease at each turn

            //------------------------------------------------------------------
            //
            // outer loop for a given Cend
            //
            //------------------------------------------------------------------
        FIND_XI:
            triplet<double>  x  = { 0, 0,    0 };           // initialize x.b=0
            triplet<double>  f  = { 0, F(0), 0 };           // initialize f.b @Cend
            const sign_type  s  = __sign::of(f.b);          // check initial sign
            search_extent    d  = search_positive_extent;   // to be set w.r.t sign
            switch(s)
            {
                case __zero__:  goto  SUCCESS;                    // early return using Cend
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

                switch(xlms.type)
                {

                    case limited_by_none: throw imported::exception(fn,"unexpected unlimited");

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
                                while( (f.a = F(x.a)) <= 0 ) x.a += x.a;
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
                                while( (f.c = F(x.c)) >= 0 ) x.c += x.c;
                                assert(x.c>0);
                                assert(f.c<0);
                                break;
                        }
                        break;

                }
            }

            //------------------------------------------------------------------
            //
            // we have a bracketed solution
            //
            //------------------------------------------------------------------
            assert(x.a<x.c);
            assert( __sign::product_of(f.a,f.c) == negative );


            // find best Cend + nu * xi
            {
                double width = fabs(x.c-x.a); assert(width>0);
            ZFIND:
                //--------------------------------------------------------------
                //
                // take middle point
                //
                //--------------------------------------------------------------
                assert(f.a>0);
                assert(f.c<0);
                f.b = F( x.b = clamp(x.a, 0.5*(x.a+x.c), x.c) ); assert(x.is_increasing());

                switch( __sign::of(f.b) )
                {
                        //------------------------------------------------------
                        //
                        // early return
                        //
                        //------------------------------------------------------
                    case __zero__:
                        comp.move(Cend,x.b);
                        goto SUCCESS;

                        //------------------------------------------------------
                        //
                        // hybrid step with f.b < 0
                        //
                        //------------------------------------------------------
                    case negative: {
                        const double den = sqrt( squared(f.b) - f.a*f.c );
                        if(fabs(f.b)<den)
                        {
                            //--------------------------------------------------
                            // Ridder's step in [x.a:x.b]
                            //--------------------------------------------------
                            const double x_r = x.b+0.5*( width * (f.b/den) );
                            const double x_u = clamp(x.a,x_r, x.b );
                            const double f_u = F(x_u);

                            switch( __sign::of(f_u) )
                            {
                                case __zero__:  
                                    comp.move(Cend,x_u);
                                    goto SUCCESS;

                                case negative:
                                    f.c = f.b = f_u;
                                    x.c = x.b = x_u;
                                    break;

                                case positive: assert(f.b<0);
                                    f.c = f.b;
                                    x.c = x.b;
                                    f.a = (f.b = f_u);
                                    x.a = (x.b = x_u);
                                    break;
                            }

                        }
                        else
                        {
                            //--------------------------------------------------
                            // fallback to bisection
                            //--------------------------------------------------
                            f.c = f.b;
                            x.c = x.b;
                        }
                    } break;

                        //------------------------------------------------------
                        //
                        // hybrid step with f.b > 0
                        //
                        //------------------------------------------------------
                    case positive: {
                        const double den = sqrt( squared(f.b) - f.a*f.c );
                        if(fabs(f.b)<den)
                        {
                            //--------------------------------------------------
                            // Ridder's step in [x.b:x.c]
                            //--------------------------------------------------
                            const double x_r = x.b+0.5*( width * (f.b/den) );
                            const double x_u = clamp(x.b,x_r, x.c );
                            const double f_u = F(x_u);

                            switch( __sign::of(f_u) )
                            {
                                case __zero__:
                                    comp.move(Cend,x_u);
                                    goto SUCCESS;

                                case negative: assert(f.b>0);
                                    f.a = f.b;
                                    x.a = x.b;
                                    f.c = (f.b = f_u);
                                    x.c = (x.b = x_u);
                                    break;

                                case positive:
                                    f.a = f.b = f_u;
                                    x.a = x.b = x_u;
                                    break;
                            }
                        }
                        else
                        {
                            //--------------------------------------------------
                            // fallback to bisection
                            //--------------------------------------------------
                            f.a = f.b;
                            x.a = x.b;
                        }
                    } break;
                }

                //--------------------------------------------------------------
                // check reduction
                //--------------------------------------------------------------
                const double new_width = fabs(x.c-x.a);
                if(new_width>=width)
                {
                    //----------------------------------------------------------
                    // end of current reduction
                    //----------------------------------------------------------
                    const double xx = x.b;
                    const double ax = fabs(xx);
                    comp.move(Cend,xx);
                    if( AX>0 && (ax>=AX) )
                        goto SUCCESS;

                    //----------------------------------------------------------
                    // prepare for next bracketing/solving
                    //----------------------------------------------------------
                    AX = ax;
                    goto FIND_XI;
                }

                //--------------------------------------------------------------
                // still reducing
                //--------------------------------------------------------------
                width = new_width;
                goto ZFIND;
            }



        SUCCESS:
            const double xi = comp.estimate_extent(Cini,Cend,xadd);
            if(call) *call  += F.n;
            return outcome(components::are_running,comp.qualify_extent(K,Cini,Cend,xmul),xi);
        }

    }

}




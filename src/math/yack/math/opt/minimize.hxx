
#include "yack/ios/ocstream.hpp"

namespace yack
{
    namespace math
    {
        static inline
        real_t grow(real_t                 xx[],
                    real_t                 ff[],
                    size_t                &nn,
                    real_function<real_t> &func,
                    const real_t           xtmp)
        {
            const real_t ans = ff[nn] = func( xx[nn] = xtmp );
            ++nn;
            return ans;
        }

        template <>
        real_t minimize:: parabolic_guess(const triplet<real_t> &x, const triplet<real_t> &f) throw()
        {
            static const real_t tiny(1e-30);
            static const real_t one(1);

            assert( x.is_increasing() );
            assert( f.a>=f.b );
            assert( f.c>=f.b );
            const real_t w     = std::abs(x.c-x.a);
            const real_t beta  = (x.b<=x.a) ? 0 : (x.c<=x.b) ? 1 : (x.b-x.a)/w;
            const real_t d_a   = std::abs(f.a-f.b)+tiny; assert(d_a>0);
            const real_t d_c   = std::abs(f.c-f.b)+tiny; assert(d_c>0);
            const real_t beta2 = beta*beta;
            const real_t num   = (one-beta2) * d_a + beta2 * d_c;
            const real_t den   = (one-beta)  * d_a + beta  * d_c;

            return clamp<real_t>(x.a,x.a+num/(den+den)*w,x.c);

        }

        //! compute sqrt(epsilon)
        template <>
        real_t minimize:: get_mtol<real_t>() throw()
        {
            static const real_t mtol = timings::round_floor( sqrt(numeric<real_t>::epsilon) );
            return mtol;
        }


        template <>
        void minimize:: move<real_t>:: run(real_function<real_t> &func,
                                           triplet<real_t>       &x,
                                           triplet<real_t>       &f)
        {
            static const real_t half(0.5);

            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert( x.is_increasing() );
            assert( f.a>=f.b );
            assert( f.c>=f.b );


            //------------------------------------------------------------------
            //
            // compute the new position u from local parabolic fit
            //
            //------------------------------------------------------------------
            real_t       xx[8] = { x.a, x.b, x.c, 0,0,0,0,0 };
            real_t       ff[8] = { f.a, f.b, f.c, 0,0,0,0,0 };
            size_t       nn    = 3;
            const real_t x_u   = parabolic_guess(x,f);

            switch( __sign::of(x.b,x_u) )
            {
                case __zero__:
                    //----------------------------------------------------------
                    // "perfect" parabolic fit, shrink interval
                    //----------------------------------------------------------
                    (void) grow(xx,ff,nn,func,clamp(x.a,x.a+(x.b-x.a)*half,x.b) );
                    (void) grow(xx,ff,nn,func,clamp(x.b,x.b+(x.c-x.b)*half,x.c) );
                    break;

                case negative: {
                    //----------------------------------------------------------
                    // x.b<x_u, x_u in [b:c]
                    //----------------------------------------------------------
                    const real_t f_u = grow(xx,ff,nn,func,x_u);
                    if(f_u<=f.b)
                    {
                        // decreasing between b and c, use second zero of parabola
                        (void) grow(xx,ff,nn,func,clamp(x_u,x_u+(x_u-x.b),x.c) );
                    }
                    else
                    {
                        // f_u>f.b , scan in [a:b]
                        (void) grow(xx,ff,nn,func,clamp(x.a,x.a+(x.b-x.a)*half,x.b));
                    }

                } break;

                case positive:
                    //----------------------------------------------------------
                    // x.b>x_u, x_u in [a:b]
                    //----------------------------------------------------------
                    assert(x.b>x_u);
                    const real_t f_u = grow(xx,ff,nn,func,x_u);
                    if(f_u<=f.b)
                    {
                        // decreasing between a and b, use second zero of parabola
                        (void) grow(xx,ff,nn,func,clamp(x.a,x_u+(x_u-x.b),x.b) );
                    }
                    else
                    {
                        // f_u>f.b, scan in [b:c]
                        (void) grow(xx,ff,nn,func,clamp(x.b,x.b+(x.c-x.b)*half,x.c));
                    }
                    break;
            }

            //------------------------------------------------------------------
            //
            // order function sampling
            //
            //------------------------------------------------------------------
            thin_array<real_t> X(xx,nn);
            thin_array<real_t> F(ff,nn);
            hsort(X,F,comparison::increasing<real_t>);

            //------------------------------------------------------------------
            //
            // number of possible triplets
            //
            //------------------------------------------------------------------
            const size_t nt    = nn-2;

            //------------------------------------------------------------------
            // find first possible one
            //------------------------------------------------------------------
            real_t       w_opt = 0;
            size_t       i_opt = 0;
            for(size_t ia=1;ia<=nt;++ia)
            {
                const size_t ib=ia+1;
                const size_t ic=ib+1;
                f.a = F[ia];
                f.b = F[ib];
                f.c = F[ic];
                if(f.b<=f.c&&f.b<=f.a)
                {
                    x.a=X[ia];
                    x.b=X[ib];
                    x.c=X[ic];
                    i_opt=ia;
                    w_opt=std::abs(x.c-x.a);
                    break;
                }
            }

            if(!i_opt) throw exception("[minimize] corrupted input!!!");

            //------------------------------------------------------------------
            // find best next interval
            //------------------------------------------------------------------
            for(size_t ia=i_opt+1;ia<=nt;++ia)
            {
                const size_t ib  = ia+1;
                const size_t ic  = ib+1;
                const real_t f_a = F[ia];
                const real_t f_b = F[ib];
                const real_t f_c = F[ic];
                if(f_b<=f_c&&f_b<=f_a)
                {
                    const real_t x_a = X[ia];
                    const real_t x_c = X[ic];
                    const real_t w_tmp = std::abs(x_c-x_a);
                    if(w_tmp<=w_opt)
                    {
                        i_opt=ia;
                        w_opt=w_tmp;
                        x.a = x_a; x.b = X[ib]; x.c = x_c;
                        f.a = f_a; f.b = f_b;   f.c = f_c;
                    }
                }
            }

        }


        

        template <>
        real_t minimize:: find<real_t>:: run(real_function<real_t> &func,
                                             triplet<real_t>       &x,
                                             triplet<real_t>       &f,
                                             const prolog           flag,
                                             real_t                 xtol)
        {
            static const real_t  ftol = numeric<real_t>::ftol;
            static const real_t  mtol = get_mtol<real_t>();

            //------------------------------------------------------------------
            //
            // initialize search
            //
            //------------------------------------------------------------------
            switch(flag)
            {
                case direct:
                    YACK_MINIMIZE("// [minimize.direct] x=" << x << ", f=" << f);
                    assert(x.is_increasing());
                    assert( f.a>=f.b );
                    assert( f.c>=f.b );
                    break;

                case inside:
                    YACK_MINIMIZE("// [minimize.inside] x=" << x << ", f=" << f);
                    if(!bracket::inside(func,x,f))
                    {
                        YACK_MINIMIZE("// [minimize] global @" << x.b);
                        return x.b; // directly set
                    }
                    assert(x.is_increasing());
                    assert( f.a>=f.b );
                    assert( f.c>=f.b );
                    break;

                case expand:
                    YACK_MINIMIZE("// [minimize.expand] x=" << x << ", f=" << f);
                    bracket::expand(func,x,f);
                    assert(x.is_increasing());
                    assert( f.a>=f.b );
                    assert( f.c>=f.b );
                    break;
            }



            real_t x_opt = x.b;
            real_t delta = std::abs(x.c-x.a);

            //------------------------------------------------------------------
            //
            // loop
            //
            //------------------------------------------------------------------
            YACK_MINIMIZE("// [minimize] x_ini=" << x << ", f_ini=" << f << ", dx=" << delta);
            while(true)
            {
                //--------------------------------------------------------------
                //
                // -  contract interval and locate x.b
                //
                //--------------------------------------------------------------
                minimize::move<real_t>::run(func,x,f);
                const real_t new_delta = std::abs(x_opt-x.b);
                x_opt=x.b;
                YACK_MINIMIZE("// [minimize] x_opt=" << x_opt << ", f_opt=" << f.b << ", dx=" << new_delta);


                //--------------------------------------------------------------
                // algorithm couldn't reduce interval
                //--------------------------------------------------------------
                if(new_delta>=delta)
                {
                    YACK_MINIMIZE("// [minimize] no better contraction");
                    break;
                }
                delta = new_delta;

                //--------------------------------------------------------------
                // user control, valid for xtol>0
                //--------------------------------------------------------------
                if(delta<=xtol)
                {
                    YACK_MINIMIZE("// [minimize] reached XTOL=" << xtol);
                    break;
                }

                //--------------------------------------------------------------
                // machine's precision control on interval
                //--------------------------------------------------------------
                if(delta<=mtol*std::abs(x_opt))
                {
                    YACK_MINIMIZE("// [minimize] reached MTOL=" << mtol);
                    break;
                }

                //--------------------------------------------------------------
                // machine's precision control on function values
                //--------------------------------------------------------------
                const real_t df = f.amplitude();
                if(df<=ftol*f.maxabs())
                {
                    YACK_MINIMIZE("// [minimize] reached FTOL=" << ftol);
                    break;
                }
            }

            //------------------------------------------------------------------
            //
            // best guess and last call
            //
            //------------------------------------------------------------------
            f.b = func(x.b=x_opt);
            YACK_MINIMIZE("// [minimize] x_end=" << x << ", f_end=" << f);
            return x.b;
            
        }

    }
}


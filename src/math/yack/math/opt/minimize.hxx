
#include "yack/ios/ocstream.hpp"
#include "yack/sequence/thin-array.hpp"

namespace yack
{
    namespace math
    {
        static inline
        real_t grow(real_t  xx[],
                    real_t  ff[],
                    size_t &nn,
                    real_function<real_t> &func,
                    const real_t           xtmp)
        {
            const real_t ans = ff[nn] = func( xx[nn] = xtmp );
            ++nn;
            return ans;
        }

        template <>
        real_t minimize:: move<real_t>:: run(triplet<real_t>       &x,
                                             triplet<real_t>       &f,
                                             real_function<real_t> &func)
        {
            static const real_t tiny(1e-30);
            static const real_t one(1);
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
            const real_t w     = fabs(x.c-x.a);
            const real_t beta  = (x.b<=x.a) ? 0 : (x.c<=x.b) ? 1 : (x.b-x.a)/w;
            const real_t d_a   = fabs(f.a-f.b)+tiny; assert(d_a>0);
            const real_t d_c   = fabs(f.c-f.b)+tiny; assert(d_c>0);
            const real_t beta2 = beta*beta;
            const real_t num   = (one-beta2) * d_a + beta2 * d_c;
            const real_t den   = (one-beta)  * d_a + beta  * d_c;
            const real_t x_u   = clamp<real_t>(x.a,x.a+num/(den+den)*w,x.c);

            real_t xx[8] = { x.a, x.b, x.c, 0,0,0,0,0 };
            real_t ff[8] = { f.a, f.b, f.c, 0,0,0,0,0 };
            size_t nn    = 3;

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

            {
                ios::ocstream fp("min-dat.dat");
                for(size_t i=1;i<=nn;++i)
                {
                    fp("%.15g %.15g\n", double(X[i]), double(F[i]-f.b));
                }
            }

            {
                ios::ocstream fp("min-fcn.dat");
                for(real_t v=0;v<=1.0;v += 0.01)
                {
                    const real_t xx = x.a + v * w;
                    fp("%g %g\n", double(xx), double(func(xx)-f.b));
                }
            }

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
                    w_opt=fabs(x.c-x.a);
                    std::cerr << "\t init x=" << x << ", f=" << f << ", w=" << w_opt << std::endl;
                    break;
                }
            }

            if(!i_opt) throw exception("corrupted minimize");

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
                    const real_t w_tmp = fabs(x_c-x_a);
                    if(w_tmp<=w_opt)
                    {
                        i_opt=ia;
                        w_opt=w_tmp;
                        x.a = x_a; x.b = X[ib]; x.c = x_c;
                        f.a = f_a; f.b = f_b;   f.c = f_c;
                    }
                }
            }

            
            return fabs(x.c-x.a);

        }

    }
}


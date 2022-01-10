
#include "yack/ios/ocstream.hpp"
#include "yack/sequence/thin-array.hpp"

namespace yack
{
    namespace math
    {
        template <>
        real_t minimize:: move<real_t>:: one_step(triplet<real_t>       &x,
                                                triplet<real_t>       &f,
                                                real_function<real_t> &F)
        {
            static const real_t tiny(1e-30);
            static const real_t one(1);
            assert( x.is_increasing() );
            assert( f.a>=f.b );
            assert( f.c>=f.b );
            assert(tiny>0);

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
            const real_t xu    = clamp<real_t>(x.a,x.a+num/(den+den)*w,x.c);

            std::cerr << "x=" << x << std::endl;
            std::cerr << "f=" << f << std::endl;
            std::cerr << "u=" << xu << std::endl;

            {
                ios::ocstream fp("min-fcn.dat");
                for(real_t v=0;v<=1.0;v += 0.01)
                {
                    const real_t xx = x.a + v * w;
                    fp("%g %g\n", double(xx), double(F(xx)));
                }
            }

            {
                real_t xx[4] = { x.a, x.b, x.c, xu    };
                real_t ff[4] = { f.a, f.b, f.c, F(xu) };
                thin_array<real_t> _x(xx,4);
                thin_array<real_t> _f(ff,4);
                hsort(_x,_f,comparison::increasing<real_t>);
                ios::ocstream fp("min-dat.dat");
                for(size_t i=0;i<4;++i)
                {
                    fp("%g %g\n", double(xx[i]), double(ff[i]));
                }
            }

            //------------------------------------------------------------------
            //
            // analyze
            //
            //------------------------------------------------------------------
            if(xu<=x.a||xu>=x.c)
            {
                return w; // limiting cases...
            }
            else
            {
                switch( __sign::of(x.b,xu) )
                {
                        //------------------------------------------------------
                    case __zero__: // didn't move
                        //------------------------------------------------------
                        return w;

                        //------------------------------------------------------
                    case negative: // xu in [b:c]
                        //------------------------------------------------------
                        assert(x.b<xu); {
                            const real_t fu = F(xu);
                            if(fu>=f.b)
                            {
                                // c->u
                                f.c = fu; x.c = xu;
                                assert(x.is_increasing()); assert(f.b<=f.a); assert(f.b<=f.c);
                            }
                            else
                            {
                                assert(fu<f.b);
                                // a->b, b->u
                                x.a = x.b; f.a = f.b;
                                x.b = xu;  f.b = fu;
                                assert(x.is_increasing()); assert(f.b<=f.a); assert(f.b<=f.c);
                            }
                        } break;

                        //------------------------------------------------------
                    case positive: // xu in [a:b]
                        //------------------------------------------------------
                        assert(xu<x.b); {
                            const real_t fu = F(xu);
                            if(fu>=f.b)
                            {
                                // a->u
                                f.a = fu; x.a = xu;
                                assert(x.is_increasing()); assert(f.b<=f.a); assert(f.b<=f.c);
                            }
                            else
                            {
                                assert(fu<f.b);
                                // c->b, b->u
                                x.c = x.b; f.c = f.b;
                                x.b = xu;  f.b = fu;
                                assert(x.is_increasing()); assert(f.b<=f.a); assert(f.b<=f.c);
                            }
                        } break;
                }
            }

            return fabs(x.c-x.a);

        }


    }
}


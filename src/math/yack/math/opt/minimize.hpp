//! \file

#ifndef YACK_OPT_MINIMIZE_INCLUDED
#define YACK_OPT_MINIMIZE_INCLUDED 1

#include "yack/math/opt/bracket.hpp"
#include "yack/math/timings.hpp"
#include "yack/math/real-function.hpp"

namespace yack
{

    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! 1D minimize
        //
        //______________________________________________________________________
        struct minimize
        {
            //__________________________________________________________________
            //
            //! making one step
            //__________________________________________________________________
            template <typename T>
            struct move
            {
                //! constrained parabolic extrapolation
                static void one_step(triplet<T>       &x,
                                     triplet<T>       &f,
                                     real_function<T> &F);

                //! wrapper to any callbale real function
                template <typename FUNC>
                static inline void one_step_(triplet<T>       &x,
                                             triplet<T>       &f,
                                             FUNC             &F)
                {
                    typename real_function_of<T>::template call<FUNC> FF(F);
                    one_step(x,f,FF);
                }

            };




            //! find minimum of a bracketed function
            /**
             * return x.b, and f.b is always the last evaluated F
             */
            template <typename T, typename FUNCTION> static inline
            T find(triplet<T> &x, triplet<T> &f, FUNCTION &F)
            {
                static const T half(0.5);
                static const T tol = timings::round_floor( sqrt(numeric<T>::epsilon) );
                //std::cerr << "tol=" << tol << std::endl;

                //--------------------------------------------------------------
                //
                // sanity check
                //
                //--------------------------------------------------------------
                assert(x.is_ordered());
                assert(f.b<=f.a);
                assert(f.b<=f.c);

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                T x0 = x.a, f0=f.a;
                T x3 = x.c, f3=f.c;
                T x1 = x.b, x2=x.b, f1=f.b, f2=f.b;
                if(fabs(x3-x1)>fabs(x1-x0))
                {
                    x.b = x2 = half*(x1+x3);
                    f.b = f2 = F(x2);
                }
                else
                {
                    x.b = x1 = half*(x0+x1);
                    f.b = f1 = F(x1);
                }
                assert(x.is_ordered());

                //--------------------------------------------------------------
                //
                // loop
                //
                //--------------------------------------------------------------
                T old_width = fabs(x3-x0);
                while(true)
                {
                    assert( (x0<=x1&&x1<=x2&&x2<=x3) || (x0>=x1&&x1>=x2&&x2>=x3) );
                    if(f1<f2)
                    {
                        x3=x2; x2=x1; x1 = half*(x0+x1);
                        f3=f2; f2=f1; f1 = F(x1);
                        x.b = x1;
                        f.b = f1;
                    }
                    else
                    {
                        x0=x1; x1=x2; x2 = half*(x2+x3);
                        f0=f1; f1=f2; f2 = F(x2);
                        x.b = x2;
                        f.b = f2;
                    }
                    const T width = fabs(x3-x0);
                    if( (width>=old_width) || (width <= tol * ( fabs(x1) + fabs(x2) )) )
                    {
                        break;
                    }
                    old_width = width;
                }

                //--------------------------------------------------------------
                //
                // finalize
                //
                //--------------------------------------------------------------
                x.a = x0; x.c = x3;
                f.a = f0; f.c = f3;
                assert(x.is_ordered());

                return x.b;
            }

        };




    }

}

#endif

//! \file

#ifndef YACK_OPT_MINIMIZE_INCLUDED
#define YACK_OPT_MINIMIZE_INCLUDED 1

#include "yack/math/opt/bracket.hpp"
#include "yack/math/timings.hpp"

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
                T width = fabs(x3-x0);
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
                    const T new_width = fabs(x3-x0);
                    //std::cerr << "width: " << width << " -> " << new_width << std::endl;

                    if( (new_width>=width) || (new_width <= tol * ( fabs(x1) + fabs(x2) )) )
                    {
                        break;
                    }
                    width = new_width;
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

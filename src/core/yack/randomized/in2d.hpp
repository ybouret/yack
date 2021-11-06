
//! \file

#ifndef YACK_RANDOMIZED_IN2D_INCLUDED
#define YACK_RANDOMIZED_IN2D_INCLUDED 1

#include "yack/randomized/bits.hpp"
 
namespace yack
{
    
    namespace randomized
    {
        struct in2d
        {
            
            template <typename T> static inline
            void on_circle(T &x, T &y, bits &ran)
            {
                while(true)
                {
                    const T x1  = ran.symm<T>();
                    const T x2  = ran.symm<T>();
                    const T x12 = x1*x1;
                    const T x22 = x2*x2;
                    const T den = x12+x22;
                    if(den>0&&den<1)
                    {
                        const T h = x1*x2;
                        x = (x12-x22)/den;
                        y = (h+h)/den;
                        return;
                    }
                }
            }
            
            template <typename T, template <typename> class VTX>
            VTX<T> on_circle(bits &ran)
            {
                T x(0),y(0);
                on_circle(x,y,ran);
                return VTX<T>(x,y);
            }
            
        };
    }
    
}

#endif


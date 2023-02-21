
#ifndef YACK_RANDOMIZED_IN3D_INCLUDED
#define YACK_RANDOMIZED_IN3D_INCLUDED 1

#include "yack/randomized/bits.hpp"

namespace yack
{

    namespace randomized
    {

        //! getting randomized values in 3D
        struct in3d
        {
            //! generate point on unit sphere
            template <typename T> static inline
            void on_sphere(T &x, T &y, T&z, bits &ran) noexcept
            {
                static const T one(1);
                while(true)
                {
                    const T x1  = ran.symm<T>();
                    const T x2  = ran.symm<T>();
                    const T x12 = x1*x1;
                    const T x22 = x2*x2;
                    const T ssq = x12+x22;
                    if(ssq<one)
                    {
                        const T del = one-ssq;
                        const T fac = sqrt(one-ssq);
                        z = del-ssq;
                        const T hx  = x1 * fac;
                        const T hy  = x2 * fac;
                        x=hx+hx;
                        y=hy+hy;
                        return;
                    }
                }
            }

            //! generate a point3d or something else on_sphere
            template <typename T, template <typename> class VTX> static inline
            VTX<T> on_sphere(bits &ran)
            {
                T x(0),y(0),z(0);
                on_sphere(x,y,z,ran);
                return VTX<T>(x,y,z);
            }

            //! generate point strictly within unit ball
            template <typename T> static inline
            void in_ball(T &x, T &y, T &z, bits &ran) noexcept
            {
                while(true)
                {
                    const T x1 = ran.symm<T>();
                    const T x2 = ran.symm<T>();
                    const T x3 = ran.symm<T>();
                    const T d2 = x1*x1 + x2*x2 + x3*x3;
                    if(d2<1)
                    {
                        x=x1;
                        y=x2;
                        z=x3;
                        return;
                    }
                }
            }

            //! generate a point3d or something else in_ball
            template <typename T, template <typename> class VTX> static inline
            VTX<T> in_ball(bits &ran)
            {
                T x(0),y(0),z(0);
                in_ball(x,y,z,ran);
                return VTX<T>(x,y,z);
            }

        };

    }

}

#endif


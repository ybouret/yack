
//! \file

#ifndef YACK_COLOR_YUV_INCLUDED
#define YACK_COLOR_YUV_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{
    namespace color
    {

#define YACK_YUV_UMIN -0.436
#define YACK_YUV_UMAX   0.436

#define YACK_YUV_VMIN -0.615
#define YACK_YUV_VMAX  0.615

        template <typename T>
        class yuv
        {
        public:
            YACK_DECL_ARGS_(T,type);

            yuv() throw();
            ~yuv() throw();
            yuv(const yuv & ) throw();
            yuv & operator=(const yuv &) throw();
            yuv(const_type Y, const_type U, const_type V) throw();

            const_type y;
            const_type u;
            const_type v;

            static inline mutable_type clamp_y(const_type Y) throw()
            {
                return (Y<0) ? 0 : ( (1<Y) ? 1 : Y);
            }

            static inline mutable_type clamp_u(const_type U) throw()
            {
                static const_type umin(YACK_YUV_UMIN);
                static const_type umax(YACK_YUV_UMAX);

                return (U<umin) ? umin : ( (umax<U) ? umax : U);
            }

            static inline mutable_type clamp_v(const_type V) throw()
            {
                static const_type vmin(YACK_YUV_VMIN);
                static const_type vmax(YACK_YUV_VMAX);

                return (V<vmin) ? vmin : ( (vmax<V) ? vmax : V);
            }

            static inline mutable_type to_y(const_type r, const_type g, const_type b) throw()
            {
                static const_type cr(0.299);
                static const_type cg(0.587);
                static const_type cb(0.114);
                return clamp_y(cr*r + cg*g + cb*b);
            }


            static inline mutable_type to_u(const_type r, const_type g, const_type b) throw()
            {
                static const_type cr(-0.147);
                static const_type cg(-0.289);
                static const_type cb(+0.437);
                return clamp_u(cr*r + cg*g + cb*b);
            }

            static inline mutable_type to_v(const_type r, const_type g, const_type b) throw()
            {
                static const_type cr(0.615);
                static const_type cg(-0.515);
                static const_type cb(-0.100);
                return clamp_v(cr*r + cg*g + cb*b);
            }



        };

    }

}

#endif

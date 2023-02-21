//#include <iostream>

namespace yack
{
    namespace color
    {
        namespace nexus
        {
            template <>
            yuv<real_t>::yuv() noexcept : y(0), u(0), v(0) {}

            template <>
            yuv<real_t>::yuv(const_type Y,
                             const_type U,
                             const_type V) noexcept :
            y(Y), u(U), v(V)
            {
#if 0
                std::cerr << "y=" << y << ", u=" << u << ", v=" << v << std::endl;
                assert(y>=0);
                assert(y<=1);

                assert(u>=YACK_YUV_UMIN);
                assert(u<=YACK_YUV_UMAX);

                assert(v>=YACK_YUV_VMIN);
                assert(v<=YACK_YUV_VMAX);
#endif
            }


            template <>
            yuv<real_t>::yuv(const yuv &other) noexcept : y(other.y), u(other.u), v(other.v) {}

            template <>
            yuv<real_t>:: ~yuv() noexcept {}

            template <>
            yuv<real_t> & yuv<real_t>::operator=(const yuv &other) noexcept {
                (real_t &) y = other.y;
                (real_t &) u = other.u;
                (real_t &) v = other.v;
                return *this;
            }

            template <>
            yuv<real_t> yuv<real_t>:: from_reals(const_type r, const_type g, const_type b) noexcept
            {
                return yuv( to_y(r,g,b), to_u(r,g,b), to_v(r,g,b) );
            }

            template <>
            yuv<real_t> yuv<real_t>:: from_bytes(const uint8_t r, const uint8_t g, const uint8_t b) noexcept
            {
                return from_reals(unit_real<real_t>::table[r],
                                  unit_real<real_t>::table[g],
                                  unit_real<real_t>::table[b]);
            }


        }

    }

}

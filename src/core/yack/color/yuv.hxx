
namespace yack
{
    namespace color
    {
        template <>
        yuv<real_t>::yuv() throw() : y(0), u(0), v(0) {}

        template <>
        yuv<real_t>::yuv(const_type Y,
                         const_type U,
                         const_type V) throw() :
        y(Y), u(U), v(V)
        {
            assert(y>=0);
            assert(y<=1);
            assert(u>=YACK_YUV_UMIN);
            assert(u<=YACK_YUV_UMAX);
            assert(v>=YACK_YUV_VMIN);
            assert(v<=YACK_YUV_VMAX);
        }
        

        template <>
        yuv<real_t>::yuv(const yuv &other) throw() : y(other.y), u(other.u), v(other.v) {}

        template <>
        yuv<real_t>:: ~yuv() throw() {}

        template <>
        yuv<real_t> & yuv<real_t>::operator=(const yuv &other) throw() {
            (real_t &) y = other.y;
            (real_t &) u = other.u;
            (real_t &) v = other.v;
            return *this;
        }

        

    }

}

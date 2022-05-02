
#include "yack/color/rgba/make-rgba.hpp"

namespace yack
{
    namespace color
    {
        make_rgba:: make_rgba() throw() :
        rgba_to_data( sizeof(rgba) )
        {
        }

        make_rgba:: ~make_rgba() throw()
        {
        }

        void make_rgba:: operator()(void *addr, const rgba &c) throw()
        {
            assert(addr);
            *static_cast<rgba *>(addr) = c;
        }
    }

}


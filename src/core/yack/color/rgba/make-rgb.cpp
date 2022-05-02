

#include "yack/color/rgba/make-rgb.hpp"

namespace yack
{
    namespace color
    {
        make_rgb:: make_rgb() throw() :
        rgba_to_data( sizeof(rgb) )
        {
        }

        make_rgb:: ~make_rgb() throw()
        {
        }

        void make_rgb:: operator()(void *addr, const rgba &c) throw()
        {
            assert(addr);
            *static_cast<rgb *>(addr) = c;
        }
    }

}


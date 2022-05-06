
#include "yack/color/rgba/from-rgba.hpp"

namespace yack
{
    namespace color
    {

        from_rgba:: ~from_rgba() throw() {}

        from_rgba::  from_rgba() throw() :
        data_to_rgba( sizeof(rgba) )
        {}

        rgba from_rgba:: operator()(const void *addr) const throw()
        {
            assert(addr);
            return *static_cast<const rgba *>(addr);
        }

    }

}


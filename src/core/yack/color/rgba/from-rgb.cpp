
#include "yack/color/rgba/from-rgb.hpp"

namespace yack
{
    namespace color
    {

        from_rgb:: ~from_rgb() throw() {}

        from_rgb::  from_rgb() throw() :
        data_to_rgba( sizeof(rgb) )
        {}

        rgba from_rgb:: operator()(const void *addr) const throw()
        {
            assert(addr);
            return rgba(*static_cast<const rgb *>(addr));
        }

    }

}


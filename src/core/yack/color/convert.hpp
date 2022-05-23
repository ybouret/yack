//! file

#ifndef YACK_COLOR_CONVERT_INCLUDED
#define YACK_COLOR_CONVERT_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace color {

        struct conversion
        {
            static const float   ftable[3*255+1];
            static const uint8_t utable[3*255+1];

        };

        template <typename T>
        struct convert
        {
            template <typename U> static
            void to(U &, const T &) throw();
        };

    }
}


#endif


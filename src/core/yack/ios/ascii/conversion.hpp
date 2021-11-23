

//! \file

#ifndef YACK_ASCII_ENCODED_INCLUDED
#define YACK_ASCII_ENCODED_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace ios
    {

        namespace ascii
        {

            struct convert
            {
                static int64_t to_int64(const char *args, size_t size, const char *ctx);
            };

        }

    }

}
#endif

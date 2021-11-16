
//! \file
#ifndef YACK_HASHING_MD_INCLUDED
#define YACK_HASHING_MD_INCLUDED 1


#include "yack/setup.hpp"

namespace yack
{

    namespace memory { class ro_buffer; }
    class digest;

    namespace hashing
    {
        class function;

        struct md
        {
            static digest of(function &);
            static digest of(function &H, const void *block_addr, const size_t block_size);
            static digest of(function &H, const char *msg);
            static digest of(function &H, const memory::ro_buffer &);

        };


    }

}

#endif


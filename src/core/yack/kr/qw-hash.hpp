//! \file

#ifndef YACK_KR_QW_HASH_INCLUDED
#define YACK_KR_QW_HASH_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace crypto
    {
        //! to remove
        struct hash64
        {
            static void des(uint32_t &lword, uint32_t &irword) throw(); //!< to remove
            static bool des_test(); //!< to remove
        };
    }
}

#endif


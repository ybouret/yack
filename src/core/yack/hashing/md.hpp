
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

        //______________________________________________________________________
        //
        //
        //! message digests
        //
        //______________________________________________________________________
        struct md
        {
            static digest of(function &H);                               //!< H content to digest
            static digest of(function &H, const void * ,const size_t  ); //!< block to digest
            static digest of(function &H, const char *);                 //!< text to digest
            static digest of(function &H, const memory::ro_buffer &);    //!< buffer to digest

        };


    }

}

#endif


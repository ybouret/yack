

//! \file

#ifndef YACK_ASCII_ENCODED_INCLUDED
#define YACK_ASCII_ENCODED_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{

    namespace ios
    {

        namespace ascii
        {

            //__________________________________________________________________
            //
            //
            //! convertion from ascii to data
            //
            //__________________________________________________________________
            struct convert
            {

                static int64_t  to_int64(const char   *args, size_t size, const char *ctx = NULL); //!< substring to int64_t
                static int64_t  to_int64(const char   *text, const char *ctx = NULL );             //!< text to int64_t
                static int64_t  to_int64(const string &data, const char *ctx = NULL);              //!< text to int64_t

                static uint64_t to_uint64(const char   *args, size_t size, const char *ctx = NULL); //!< substring to uint64_t
                static uint64_t to_uint64(const char   *text, const char *ctx = NULL);              //!< text to uint64_t
                static uint64_t to_uint64(const string &data, const char *ctx = NULL);              //!< text to uint64_t

            };

        }

    }

}
#endif

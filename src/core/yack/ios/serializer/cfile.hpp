
//! \file

#ifndef YACK_IOS_SER_CFILE_INCLUDED
#define YACK_IOS_SER_CFILE_INCLUDED 1

#include "yack/ios/serializable.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{
    namespace ios
    {
        namespace serializer
        {
            //! saving to
            struct cfile
            {
                //! save to file, return written bytes
                static size_t save(const char   *, const serializable &, const bool append=false);
              
                //! save to file, return written bytes
                static size_t save(const string &, const serializable &, const bool append=false);

            };
        }
    }
}

#endif


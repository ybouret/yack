
//! \file

#ifndef YACK_IOS_SER_BINARY_INCLUDED
#define YACK_IOS_SER_BINARY_INCLUDED 1

#include "yack/ios/serializable.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{
    namespace ios
    {
        namespace serializer
        {
            //! convert to binary representation
            struct to_binary
            {
                static string from(const serializable &obj); //!< make a binary string
            };
        }
    }
}

#endif


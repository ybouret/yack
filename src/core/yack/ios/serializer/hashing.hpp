//! \file

#ifndef YACK_IOS_SER_HASHING_INCLUDED
#define YACK_IOS_SER_HASHING_INCLUDED 1

#include "yack/ios/serializable.hpp"
#include "yack/hashing/function.hpp"

namespace yack
{
    namespace ios
    {
        namespace serializer
        {
            struct hash
            {
                //! directly run object into hasher, no set
                /**
                 * \return the number of written bytes
                 */
                static size_t run(hashing::function &h, const serializable &obj);
            };
        }
    }
}

#endif


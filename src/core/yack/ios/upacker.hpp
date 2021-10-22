

//! \file
#ifndef YACK_IOS_UPACKER_INCLUDED
#define YACK_IOS_UPACKER_INCLUDED 1

#include "yack/ios/fwd.hpp"

namespace yack
{
    namespace ios
    {
        
        struct upacker
        {
            static size_t encode(uint8_t b[], uint64_t qw) throw();
        };
    }
    
}

#endif

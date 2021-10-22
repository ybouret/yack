

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
            static const unsigned prolog_bits = 4; //!< 0..9 bytes
            static const unsigned prolog_shft = 8 - prolog_bits;
            static size_t write64(ostream &, const uint64_t);
            
        };
        
    }
    
}

#endif

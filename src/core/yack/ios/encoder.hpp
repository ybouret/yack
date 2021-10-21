
//! \file

#ifndef YACK_IOS_ENCODER_INCLUDED
#define YACK_IOS_ENCODER_INCLUDED 1

#include "yack/ios/ostream.hpp"

namespace yack
{
    namespace ios
    {

        struct encoder
        {

            static void put(ostream &, const uint8_t  &);
            static void put(ostream &, const uint16_t &);
            static void put(ostream &, const uint32_t &);
            static void put(ostream &, const uint64_t &);
            
            template <typename T> static inline
            size_t emit(ostream &os,const T x)
            {
                const typename unsigned_for<T>::type y(x);
                put(os,y);
                return sizeof(T);
            }


        };

    }

}

#endif

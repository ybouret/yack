

//! \file

#ifndef YACK_IOS_DECODER_INCLUDED
#define YACK_IOS_DECODER_INCLUDED 1

#include "yack/ios/istream.hpp"
#include "yack/type/ints.hpp"

namespace yack
{
    namespace ios
    {

        struct decoder
        {
            
            static bool get(istream &is, uint8_t &x);
            static bool get(istream &is, uint16_t &x);
            static bool get(istream &is, uint32_t &x);
            static bool get(istream &is, uint64_t &x);


            template <typename T> static inline
            size_t read(istream &is,T &x)
            {
                typedef typename unsigned_for<T>::type utype;
                if( get(is, (utype&)x ) )
                {
                    return sizeof(T);
                }
                else
                {
                    return 0;
                }

            }

        };

    }

}

#endif

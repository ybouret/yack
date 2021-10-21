

//! \file

#ifndef YACK_IOS_DECODER_INCLUDED
#define YACK_IOS_DECODER_INCLUDED 1

#include "yack/ios/istream.hpp"
#include "yack/type/ints.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! decoding operations
        //
        //______________________________________________________________________
        struct decoder
        {
            
            static bool get(istream &is, uint8_t  &x); //!< try to get 1 byte
            static bool get(istream &is, uint16_t &x); //!< try to get 2 bytes
            static bool get(istream &is, uint32_t &x); //!< try to get 4 bytes
            static bool get(istream &is, uint64_t &x); //!< try to get 8 bytes


            //! read corresponding bytes
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

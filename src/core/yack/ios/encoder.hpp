
//! \file

#ifndef YACK_IOS_ENCODER_INCLUDED
#define YACK_IOS_ENCODER_INCLUDED 1

#include "yack/ios/ostream.hpp"
#include "yack/type/ints.hpp"

namespace yack
{
    namespace ios
    {

        //______________________________________________________________________
        //
        //
        //! encoding operation
        //
        //______________________________________________________________________
        struct encoder
        {

            static void put(ostream &, const uint8_t  &); //!< put one byte
            static void put(ostream &, const uint16_t &); //!< put 2 bytes
            static void put(ostream &, const uint32_t &); //!< put 4 bytes
            static void put(ostream &, const uint64_t &); //!< put 8 bytes

            //! write the matching number of bytes
            template <typename T> static inline
            size_t emit(ostream &os,const T x)
            {
                const typename unsigned_for<T>::type y(x);
                put(os,y);
                return sizeof(T);
            }

            static size_t upack64(ostream &, uint64_t);


        };

    }

}

#endif

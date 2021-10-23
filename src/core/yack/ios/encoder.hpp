
//! \file

#ifndef YACK_IOS_ENCODER_INCLUDED
#define YACK_IOS_ENCODER_INCLUDED 1

#include "yack/type/ints.hpp"
#include "yack/container/readable.hpp"

namespace yack
{
    namespace ios
    {
        class ostream;

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


            //! transform qw in [0..9] bytes
            static size_t upack(uint8_t *, uint64_t qw) throw();

            //! serialize 64bits by quartets
            static size_t serialize64(ostream &, const uint64_t);

            //! serialize an integer type
            template <typename T> static inline
            size_t serialize(ostream &os, const T x)
            {
                const typename unsigned_for<T>::type y(x);
                return serialize64(os,y);
            }

            //! serialize a readable array of integral types
            template <typename T> static inline
            size_t serialize(ostream &os, const readable<T> &arr)
            {
                const size_t sz = arr.size();
                size_t       nw = serialize<size_t>(os,sz);
                for(size_t i=1;i<=sz;++i)
                {
                    nw += emit<T>(os,arr[i]);
                }
                return nw;
            }


        };

    }

}

#endif

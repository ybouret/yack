

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
            size_t read(istream &input,T &x)
            {
                typedef typename unsigned_for<T>::type utype;
                if( get(input, (utype&)x ) )
                {
                    return sizeof(T);
                }
                else
                {
                    return 0;
                }
            }

            //! construct a previously serialized value
            static size_t construct64(istream &fp, uint64_t &value, const char *info);

            template <typename T> static inline
            size_t construct(istream    &fp,
                             T          &value,
                             const char *info=0)
            {
                static const size_t max_bits = sizeof(T) * 8 ;
                uint64_t     x  = 0;
                const size_t nr = construct64(fp,x,info);
                check_bits(x,max_bits,info);
                const typename unsigned_for<T>::type y(x);
                value = T(y);
                return nr;
            }


        private:
            static void check_bits(const uint64_t x,
                                   const size_t   max_bits,
                                   const char *info);

        };

    }

}

#endif

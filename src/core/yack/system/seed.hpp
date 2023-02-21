
//! \file
#ifndef YACK_SYSTEM_SEED_INCLUDED
#define YACK_SYSTEM_SEED_INCLUDED 1

#include "yack/type/ints.hpp"
#include "yack/arith/base2.hpp"
#include <iostream>
namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! making a seed from wtime and process id
    //
    //__________________________________________________________________________
    struct system_seed
    {
        static uint64_t get64() noexcept; //!< create a hashed value

        //! make a positive seed from 64 bits
        template <typename T>
        static inline T get() noexcept
        {
            static const uint64_t max64 = integral_for<T>::maximum;
            static const size_t   bits  = bits_for(max64);

            uint64_t seed64 = get64();
            T        seed   = 0;
            for(size_t i=bits;i>0;--i)
            {
                seed <<= 1;
                if(0!=(seed64&1)) seed |= 1;
                seed64 >>= 1;
            }
            return seed;
        }

    };

}


#endif

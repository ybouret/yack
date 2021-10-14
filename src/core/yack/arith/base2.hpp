

//! \file
#ifndef Y_ARITH_BASE2XX_INCLUDED
#define Y_ARITH_BASE2XX_INCLUDED 1

#include "yack/type/ints.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! base2 operations
    //
    //__________________________________________________________________________
    template <typename type>
    struct base2
    {
        static const bool     signed_value     = is_signed<type>::value;                 //!< alias
        static const unsigned max_shift        = (sizeof(type)<<3) - (signed_value?2:1); //!< alias
        static const unsigned max_slots        = max_shift+1;                            //!< alias
        static const type     one              = 1;                                      //!< alias
        static const type     max_power_of_two = (one << max_shift);                     //!< alias


        //______________________________________________________________________
        //
        //! compute r such that v <= (1<<r)
        //__________________________________________________________________________
        static inline unsigned log2_of(type v) throw()
        {
            assert(v>0);
            unsigned r=0;
            while( (v>>=1) > 0 ) ++r;
            assert( v <= (one<<r) );
            return r;
        }


    };


    //! power of two detection
    template <typename T> inline
    bool is_a_power_of_two( T v ) throw()
    {
        return (v>0) && !( v & (v - base2<T>::one) );
    }

    //! next power of two of v<=max_power_of_two
    template <typename T>
    inline T next_power_of_two( T v ) throw()
    {
        if(v>0)
        {
            assert(v<=base2<T>::max_power_of_two);
            --v;
            for( size_t shift = 1; shift <= (sizeof(T)<<2); shift <<= 1 )
            {
                v |= ( v >> shift );
            }
            return ++v;
        }
        else
        {
            return base2<T>::one;
        }
    }

    //! most_significant_bit of v>=0
    template <typename T>
    inline T most_significant_bit(const T v) throw()
    {
        T mask = base2<T>::max_power_of_two;
        while(mask)
        {
            if(0!=(v&mask)) return mask;
            mask >>= 1;
        }
        return 0;
    }

    //! previous power of two, v>0
    template <typename T>
    inline T prev_power_of_two( const T v ) throw()
    {
        assert(v>0);
        T mask = base2<T>::max_power_of_two;
        while(true)
        {
            if(0!=(v&mask)) return mask;
            mask >>= 1;
        }
    }


    //! v <= (1<<shift)
    template <typename T>
    inline unsigned integer_log2( const T v ) throw()
    {
        return base2<T>::log2_of(v);
    }

    //! counting byte for value
    template <typename T>   size_t bytes_for(const T) throw();

    template <> size_t bytes_for<uint8_t>(const  uint8_t  b) throw(); //!< for 8bits
    template <> size_t bytes_for<uint16_t>(const uint16_t b) throw(); //!< for 16bits
    template <> size_t bytes_for<uint32_t>(const uint32_t b) throw(); //!< for 32bits
    template <> size_t bytes_for<uint64_t>(const uint64_t b) throw(); //!< for 64bits


    //! counting number of bits for value
    template <typename T> size_t bits_for(const T b) throw()
    {
        T      ibit = base2<T>::max_power_of_two;
        size_t nbit = sizeof(T)*8;
        while(nbit>0)
        {
            if(0!=(ibit&b)) return nbit;
            ibit >>= 1;
            --nbit;
        }
        return 0;
    }


}
#endif


//! \file

#ifndef Y_ARITH_BASE10_INCLUDED
#define Y_ARITH_BASE10_INCLUDED 1

#include "yack/type/ints.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! base10 operations
    //
    //__________________________________________________________________________
    template <typename T>
    struct base10
    {
        static const T ten = 10; //!< alias
    };

    //! count digits to display value
    template <typename T>
    inline unsigned digits_for(T value) throw()
    {
        if(value < base10<T>::ten)
        {
            return 1;
        }
        else
        {
            unsigned res = 1;
            while(value>=base10<T>::ten)
            {
                ++res;
                value /= base10<T>::ten;
            }
            return res;
        }
    }


}

#endif

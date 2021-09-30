

//! \file

#ifndef YACK_ALIGNXX_INCLUDED
#define YACK_ALIGNXX_INCLUDED 1

#include "yack/arith/align.h"
#include "yack/arith/ilog2.hpp"

namespace yack
{
    //! TODO
    struct alignment
    {
        //! TODO
        static size_t with_ln2(const size_t ln2, const size_t value) throw();

        //! TODO
        template <size_t POT> static inline
        size_t on(const size_t n)
        {
            //static const size_t ln2  = ilog2<POT>::value;
            //static const size_t byte = POT;
            static const size_t mask = POT-1;
            static const size_t not2 =  ~mask;
            return (n+mask) & not2;
        }
        
    };
    
    
}

#endif


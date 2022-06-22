//! \file

#ifndef YACK_IOTA_INCLUDED
#define YACK_IOTA_INCLUDED 1

#include "yack/setup.hpp"

namespace yack {

    namespace math
    {
        //______________________________________________________________________
        //
        //
        //! Inline Operations of Template Algebra
        //
        //______________________________________________________________________
        struct iota
        {

            //------------------------------------------------------------------
            //! target[1..target.size()] = source[1..target.size()]
            //------------------------------------------------------------------
            template <typename TARGET, typename SOURCE> static inline
            void set(TARGET &target, SOURCE &source)
            {
                assert(source.size()>=target.size());
                for(size_t i=source.size();i>0;--i) target[i] = static_cast< typename TARGET::type >( source[i] );
            }



        };

    }

}

#endif


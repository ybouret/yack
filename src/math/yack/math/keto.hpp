
//! \file

#ifndef YACK_KETO_INCLUDED
#define YACK_KETO_INCLUDED 1

#include "yack/setup.hpp"

namespace yack {

    namespace math
    {
        //! Kernel for Easy Template Operations
        struct keto
        {
            //! target[1:target.size()] = source[1:target.size()]
            template <typename TARGET, typename SOURCE> static inline
            void load(TARGET &target, SOURCE &source)
            {
                assert(source.size()>=target.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] = source[i];
            }

        };
    }

}

#endif


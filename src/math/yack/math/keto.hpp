
//! \file

#ifndef YACK_KETO_INCLUDED
#define YACK_KETO_INCLUDED 1

#include "yack/ipso/adding.hpp"
#include "yack/container/matrix.hpp"

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

            //! target[1:source.size()] = source[1:source.size()]
            template <typename TARGET, typename SOURCE> static inline
            void save(TARGET &target, SOURCE &source)
            {
                assert(target.size()>=source.size());
                for(size_t i=source.size();i>0;--i)
                    target[i] = source[i];
            }

            template <typename TARGET, typename T, typename SOURCE> static inline
            void mul(TARGET          & target,
                     const matrix<T> & M,
                     SOURCE          & source)
            {
                assert( target.size() >= M.rows );
                assert( source.size() >= M.cols );
                
            }


        };
    }

}

#endif

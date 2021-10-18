
//! \file

#ifndef YACK_SYNC_SPLIT_INCLUDED
#define YACK_SYNC_SPLIT_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace concurrent
    {

        struct split
        {
            
            template <typename T, typename U> static inline
            void with(const T size, const T rank, U &length, U &offset) throw()
            {
                assert(size>0);
                assert(rank<size);

                T left = size;
                U todo = length/left;
                for(size_t r=0;r<rank;++r)
                {
                    --left;
                    length -= todo;
                    offset += todo;
                    todo = length/left;
                }
                length = todo;


            }

        };

    }

}

#endif

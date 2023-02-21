
//! \file

#ifndef YACK_SYNC_SPLIT1D_INCLUDED
#define YACK_SYNC_SPLIT1D_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! generic splitting 1D algorithm
        //
        //______________________________________________________________________
        struct split1D
        {

            //! from total length and offset, return parameters for size.rank
            template <typename T, typename U> static inline
            void with(const T size, const T rank, U &length, U &offset) noexcept
            {
                assert(size>0);
                assert(rank<size);

                T left = size;
                U todo = length/left;
                for(T r=0;r<rank;++r)
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

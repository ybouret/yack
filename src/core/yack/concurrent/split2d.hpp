

//! \file

#ifndef YACK_SYNC_SPLIT2D_INCLUDED
#define YACK_SYNC_SPLIT2D_INCLUDED 1

#include "yack/concurrent/split1d.hpp"

namespace yack
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! generic splitting 2D algorithm
        //
        //______________________________________________________________________
        struct split2D
        {

            //! from ...
            template <typename T, typename U> static inline
            void with(const T size,
                      const T rank,
                      U     &  lower_row,
                      U     &  upper_row,
                      U     &  lower_col,
                      U     &  upper_col) throw()
            {
                assert(size>0);
                assert(rank<size);
                assert(lower_row<=upper_row);
                assert(lower_col<=upper_col);
                const uint64_t rows = 1+(upper_row-lower_row);
                const uint64_t cols = 1+(upper_col-lower_col);
                uint64_t       length = rows*cols;
                uint64_t       offset = 0;
                split1D::with(size,rank,length,offset);
            }

        };

    }

}

#endif



//! \file

#ifndef YACK_SYNC_SPLIT2D_INCLUDED
#define YACK_SYNC_SPLIT2D_INCLUDED 1

#include "yack/concurrent/split1d.hpp"

namespace yack
{
    namespace concurrent
    {
        
        template <typename T>
        class tile
        {
        public:
            const T row;
            const T column;
            const T length;
            
        private:
            YACK_DISABLE_ASSIGN(tile);
        };
        
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
                      const T rank)
            {
            }
                      

        };

    }

}

#endif

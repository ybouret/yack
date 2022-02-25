
//! \file

#ifndef YACK_GFX_BITROW_INCLUDED
#define YACK_GFX_BITROW_INCLUDED 1

#include "yack/gfx/zero-flux.hpp"

namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! row for bitmap
        //
        //______________________________________________________________________
        class bitrow
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            bitrow(void            *entry,
                   const zero_flux &zfluz) throw();

            ~bitrow() throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
        private: void            *p;
        public:  const zero_flux &z;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bitrow);
        };

    }

}

#endif

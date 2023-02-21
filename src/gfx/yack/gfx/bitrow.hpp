
//! \file

#ifndef YACK_GFX_BITROW_INCLUDED
#define YACK_GFX_BITROW_INCLUDED 1

#include "yack/gfx/zero-flux.hpp"

namespace yack
{
    namespace graphic
    {

        typedef raw_zero_flux zero_flux; //!< alias
        
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

            //! setup
            bitrow(void            *entry,
                   const zero_flux &zfluz) noexcept;

            //! cleanup
            ~bitrow() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const uint8_t *addr() const noexcept; //!< first const byte
            uint8_t       *addr()       noexcept; //!< first byte

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
        private: void            *p; //!< first item address
        public:  const zero_flux &w; //!< zero_flux computation reference



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bitrow);
        };

    }

}

#endif

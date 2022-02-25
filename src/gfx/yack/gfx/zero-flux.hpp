//! \file

#ifndef YACK_GFX_ZERO_FLUX_INCLUDED
#define YACK_GFX_ZERO_FLUX_INCLUDED 1

#include "yack/setup.hpp"


namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! compute indices to produce zero flux operators
        //
        //______________________________________________________________________
        class zero_flux
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            zero_flux(const unit_t    n) throw();
            zero_flux(const zero_flux &) throw();
            ~zero_flux() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            unit_t operator()(unit_t indx) const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t size;
            const unit_t szsz;

        private:
            YACK_DISABLE_ASSIGN(zero_flux);
        };

    }

}


#endif


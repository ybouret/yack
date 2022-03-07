//! \file

#ifndef YACK_ARITH_RAW_ZFLUX_INCLUDED
#define YACK_ARITH_RAW_ZFLUX_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! compute indices to produce zero flux operators
    //
    //__________________________________________________________________________
    class raw_zero_flux
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        raw_zero_flux(const unit_t        n) throw();
        raw_zero_flux(const raw_zero_flux &) throw();
        ~raw_zero_flux() throw();

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        unit_t operator()(unit_t indx) const throw();

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const unit_t size;
        const unit_t szsz;

    private:
        YACK_DISABLE_ASSIGN(raw_zero_flux);
    };

}

#endif


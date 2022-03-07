//! \file

#ifndef YACK_ARITH_CXX_ZFLUX_INCLUDED
#define YACK_ARITH_CXX_ZFLUX_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! compute indices to produce zero flux operators, offset 1
    //
    //__________________________________________________________________________
    class cxx_zero_flux
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        cxx_zero_flux(const unit_t        n) throw();
        cxx_zero_flux(const cxx_zero_flux &) throw();
        ~cxx_zero_flux() throw();

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
        YACK_DISABLE_ASSIGN(cxx_zero_flux);
    };

}

#endif


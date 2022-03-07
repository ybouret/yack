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
        // definition
        //______________________________________________________________________
        typedef unit_t (raw_zero_flux::*proc)(unit_t) const;

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
        unit_t operator()(const unit_t indx) const throw();

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const unit_t size;
        const unit_t szsz;

    private:
        YACK_DISABLE_ASSIGN(raw_zero_flux);
        proc   get_;
        unit_t get1(unit_t) const throw();
        unit_t getn(unit_t) const throw();
    };

}

#endif


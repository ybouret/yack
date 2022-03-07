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
        // definition
        //______________________________________________________________________
        typedef unit_t (cxx_zero_flux::*proc)(const unit_t) const;

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
        unit_t operator()(const unit_t indx) const throw();

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const unit_t size;
        const unit_t szsz;

    private:
        YACK_DISABLE_ASSIGN(cxx_zero_flux);
        proc        get_;
        unit_t      get1(const unit_t) const throw();
        unit_t      getn(const unit_t) const throw();
    };

}

#endif


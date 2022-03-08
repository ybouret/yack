
//! \file

#ifndef YACK_ARITH_RAW_ZFLUX_INCLUDED
#define YACK_ARITH_RAW_ZFLUX_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace core
    {
        //______________________________________________________________________
        //
        //
        //! compute indices to produce zero flux operators
        //
        //______________________________________________________________________
        class  zero_flux
        {
        public:
            //__________________________________________________________________
            //
            // definition
            //__________________________________________________________________
            typedef unit_t (*proc)(const zero_flux &, unit_t);

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            zero_flux(const unit_t) throw();
            zero_flux(const zero_flux&) throw();
            ~zero_flux() throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t size; //!< size
            const unit_t szsz; //!< size*2

            typedef proc (*get_proc_for)(const zero_flux &);
            static proc raw_proc_for(const zero_flux &self) throw(); //!< to map [0..size-1]
            static proc cxx_proc_for(const zero_flux &self) throw(); //!< to map [1..size]


        private:
            YACK_DISABLE_ASSIGN(zero_flux);

        };
    }

    template <core::zero_flux::get_proc_for PROC>
    class zero_flux_using
    {
    public:
        typedef core::zero_flux flux_type;
        typedef flux_type::proc proc_type;
        
        inline  zero_flux_using(const unit_t           n) throw() : data(n),      proc( PROC(data) ) {}
        inline  zero_flux_using(const zero_flux_using &z) throw() : data(z.data), proc(z.proc)       {}
        inline ~zero_flux_using()                         throw() {}

    private:
        const flux_type data;
        const proc_type proc;
    };
    
#if 0
    class raw_zero_flux
    {
    public:
        typedef core::zero_flux flux_type;
        typedef flux_type::proc proc_type;

        inline  raw_zero_flux(const unit_t         n) throw() : data(n), proc( flux_type::raw_proc_for(data) ) {}
        inline  raw_zero_flux(const raw_zero_flux &z) throw() : data(z.data), proc(z.proc) {}
        inline ~raw_zero_flux() throw() { coerce(proc) = 0; }

        inline unit_t operator()(const unit_t indx) const throw()
        {
            return proc(data,indx);
        }

    private:
        YACK_DISABLE_ASSIGN(raw_zero_flux);
        const flux_type data;
        const proc_type proc;
    };
#endif

};

#endif

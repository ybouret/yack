
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
            typedef unit_t (*proc)(const zero_flux &, unit_t); //!< alias
            typedef proc  (*get_proc_for)(const zero_flux &);  //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            zero_flux(const unit_t)     throw(); //!< setup
            zero_flux(const zero_flux&) throw(); //!< copy
            ~zero_flux() throw();                //!< cleanup

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t size; //!< size
            const unit_t szsz; //!< size*2

            static  proc raw_proc_for(const zero_flux &self) throw(); //!< to map [0..size-1]
            static  proc cxx_proc_for(const zero_flux &self) throw(); //!< to map [1..size]


        private:
            YACK_DISABLE_ASSIGN(zero_flux);

        };
    }

    //__________________________________________________________________________
    //
    //
    //! zero flux using one procedure to choose output range
    //
    //__________________________________________________________________________
    template <core::zero_flux::get_proc_for PROC>
    class zero_flux_using
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef core::zero_flux flux_type; //!< alias
        typedef flux_type::proc proc_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  zero_flux_using(const unit_t           n) throw() : data(n),      proc( PROC(data) ) {} //!< setup
        inline  zero_flux_using(const zero_flux_using &z) throw() : data(z.data), proc(z.proc)       {} //!< copy
        inline ~zero_flux_using()                         throw() {}                                    //!< cleanup

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline unit_t operator*()                   const throw() { return data.size; }       //!< size
        inline unit_t operator()(const unit_t indx) const throw() { return proc(data,indx); } //!< output parameter


    private:
        const flux_type data;
        const proc_type proc;
    };

    //__________________________________________________________________________
    //
    //
    // aliases
    //
    //__________________________________________________________________________
    typedef zero_flux_using<core::zero_flux::raw_proc_for> raw_zero_flux; //!< using [0..size-1]
    typedef zero_flux_using<core::zero_flux::cxx_proc_for> cxx_zero_flux; //!< using [1..size]
    
};

#endif

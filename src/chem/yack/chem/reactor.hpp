

//! \file

#ifndef YACK_CHEM_REACTOR_INCLUDED
#define YACK_CHEM_REACTOR_INCLUDED 1

#include "yack/chem/types.hpp"
#include "yack/chem/mix/active.hpp"
#include "yack/chem/mix/groups.hpp"

namespace yack
{
    namespace chemical
    {

        
        //______________________________________________________________________
        //
        //
        //! chemical reactor
        //
        //______________________________________________________________________
        class reactor
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char  clid[];  //!< identifier
            static const bool &verbose; //!< reference on entity::verbose
            static const char  vpfx[];  //!< prefix for verbosity

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! initialize from lib/eqs at tini to compute initial constants
            explicit reactor(library     &lib_,
                             equilibria  &eqs_,
                             const double t0);
            virtual ~reactor() throw(); //!< cleanup



            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library    &lib;     //!< user's library of species
            const library     sub;     //!< duplicate library to build database
            const equilibria &singles; //!< user's equilibria
            const equilibria  couples; //!< all possible couples
            const equilibria  lattice; //!< singles+couples
            const size_t      M;       //!< number of species
            const size_t      N;       //!< number of equilibria
            const size_t      L;       //!< lattice size
            const alist       active;  //!< list of active species
            const imatrix     Nu;      //!< [NxM] topology matrix

        private:
            tableaux ntab; //!< for [N]
            tableaux mtab; //!< for [M]
            tableaux ltab; //!< for [L]

        public:
            tableau &K; //!< [N]


            tableau &Kl; //!< [L] all constants of lattice
            rmatrix  Cl; //!< [LxM] all equilibria of lattice


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
            const lockable::scope libLock;
            const lockable::scope eqsLock;
        };

    }
}

#endif


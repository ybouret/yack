

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

            //! solve with precomputed K
            bool solve(writable<double> &C0) throw();


            double hamiltonian(const readable<double> &C) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library         &lib;      //!< user's library of species
            const library          sub;      //!< duplicate library to build database
            const equilibria      &singles;  //!< user's equilibria
            const equilibria       couples;  //!< all possible couples
            const equilibria       lattice;  //!< singles+couples
            const auto_ptr<groups> look_up;  //!< parallel combinations
            const size_t            M;       //!< number of species
            const size_t            N;       //!< number of equilibria
            const size_t            L;       //!< lattice size
            const alist             active;  //!< list of active species
            const imatrix           Nu;      //!< [NxM] topology matrix
            imatrix                 NuA;     //!< [NxM] Active Topology
            rmatrix                 Psi;     //!< [NxM] jacobian
            rmatrix                 Omega0;  //!< [NxN] system matrix
            rmatrix                 iOmega;  //!< [NxN] decomposed Omega0

        private:
            auto_ptr<rsolver>       LU;
            tableaux ntab; //!< for [N]
            tableaux mtab; //!< for [M]
            tableaux ltab; //!< for [L]

        public:
            tableau &K;       //!< [N]
            tableau &Xend;    //!< [N]
            tableau &Xtry;    //!< [N]
            tableau &Gamma;   //!< [N]
            tableau &xi;      //!< [N]
            booltab blocked;  //!< [N]

            tableau &Corg; //!< [M]
            tableau &Cend; //!< [M]
            tableau &Ctry; //!< [M]
            tableau &Cstp; //!< [M]

            vector<double> ratio;

            tableau &Kl; //!< [L] all constants of lattice
            tableau &Xl; //!< [L] all Xi of lattice
            booltab  Ok; //!< [L] decreasing hamiltonian
            rmatrix  Cl; //!< [LxM] all equilibria of lattice


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
            const lockable::scope libLock;
            const lockable::scope eqsLock;


            bool onSuccess(writable<double> &C0);

            double singlesXi(const double G0) throw(); //!< return |Xi|
            void   couplesXi(const double G0) throw();
            double singlesXi() throw(); //!< update |Xi|

            bool   underflowing() throw();

            bool   decreaseHamiltonian(double &G0) throw();
            double combinedHamiltonian(const group &g, writable<double> &C) throw();

            void   suspendEquilibrium(const size_t ei) throw();
            size_t initializeLocalStep() throw();
            double optimizeLocalStep(const double G0) throw();

        };

    }
}

#endif


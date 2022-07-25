

//! \file

#ifndef YACK_CHEM_REACTOR_INCLUDED
#define YACK_CHEM_REACTOR_INCLUDED 1

#include "yack/chem/types.hpp"
#include "yack/chem/mix/active.hpp"
#include "yack/chem/mix/groups.hpp"
#include "yack/sort/network/sort.hpp"

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

            bool  solve(writable<double> &C0) throw();

            bool  steady(writable<double> &C0) throw();

            void  display(const readable<double> &C0);

            double  operator()(const double u) throw(); //!< hamiltonian( Corg * (1-u) + Cend * u )

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const network_sort    &nws;      //!< network sorting
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
            tableau &K;       //!< [N] constants for singles
            tableau &Xtry;    //!< [N] trial values
            tableau &Gamma;   //!< [N]
            tableau &xi;      //!< [N]
            tableau &sigma;   //!< [N] - <Psi_i|Nu_i>

            tableau &Corg; //!< [M]
            tableau &Cend; //!< [M]
            tableau &Ctry; //!< [M]
            tableau &Cstp; //!< [M]

            vector<double> ratio; //!< limiting ratio

            tableau &Kl;  //!< [L] all constants of lattice
            tableau &Xl;  //!< [L] all Xi of lattice
            rmatrix  Cl;  //!< [LxM] all equilibria of lattice
            booltab  blocked;  //!< [L]


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
            const lockable::scope libLock;
            const lockable::scope eqsLock;



            const equilibrium  *maximumOfSingles(size_t &nrun)    throw(); //!< construct sigma, update nrun, true is |Xi|<=0
            const equilibrium  *minimumOfLattice(const double G0) throw();

            bool               querySingles(size_t &nrun)    throw();
            const equilibrium *queryLattice(const double G0) throw();
            double             Hamiltonian(const readable<double> &C) throw();
            double             Htry(const double G0)                  throw();

            bool         successfulUpdate(writable<double> &C0, unsigned cycle) throw(); //!< transfer + verbose info
            bool         initializeSearch(const readable<double> &C0)                 throw(); //!< initialize, true=>success
            bool         updateSuccessful(writable<double> &C0, unsigned cycle) throw(); //!< transfer + verbose info
            void         initializeOmega0()                                     throw(); //!< jacobian
            double       mixedHamiltonian(writable<double> &C, const group &g)  throw(); //!< aggregate a mixed combination
            double       buildHamiltonian(const equilibrium &eq)                throw(); //!< build best from eq
            void         zapEquilibriumAt(const size_t ei)                      throw(); //!< de-activate
            bool         optimizeFullStep(const double G0)                      throw(); //!< numerical look up
            bool         validSteadyState() throw();

            static const char *accepting(const bool flag) throw(); //!< helper
        };

    }
}

#endif


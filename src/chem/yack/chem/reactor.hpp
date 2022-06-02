//! \file

#ifndef YACK_CHEM_REACTOR_INCLUDED
#define YACK_CHEM_REACTOR_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/container/matrix.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/sequence/arrays.hpp"

namespace yack
{
    namespace chemical
    {


        typedef matrix<int>            imatrix;  //!< alias
        typedef matrix<double>         rmatrix;  //!< alias
        typedef vector<double>         rvector;  //!< alias
        typedef arrays_of<double>      tableaux; //!< alias
        typedef tableaux::array_type   tableau;  //!< alias
        typedef thin_array<bool>       booltab;  //!< alias

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
            static const char  clid[];  //!< "chemical::reactor"
            static const bool &verbose; //!< on entity verbose
            static const char  vpfx[];  //!< prefix for verbosity

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! initialize from lib/eqs at t0 to compute initial constants
            explicit reactor(library     &lib_,
                             equilibria  &eqs_,
                             const double t0);
            virtual ~reactor() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! transfer active species only
            void transfer(writable<double> &, const readable<double> & ) const throw();

            //! check valid concentrations
            bool are_valid(const readable<double>  &C) const throw();

            //! careful sum(Gamma^2)/B
            double meanGammaSquared(const readable<double> &C) throw();

            //! meanGammaSquared( (1-u)*Corg + u * Cend)
            double operator()(const double u) throw();

            //! precompute all K
            void computeK(const double t);

            //! solve...
            bool solve(writable<double> &C0) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library    &lib;     //!< user's lib
            const equilibria &eqs;     //!< user's eqs
            const library     sub;     //!< copy of lib
            const size_t      M;       //!< number of species
            const size_t      MA;      //!< number of active species
            const size_t      MP;      //!< number of primary species
            const size_t      N;       //!< number of equilibria
            const size_t      NC;      //!< number of couples
            const size_t      Ntot;    //!< N+NC
            const equilibria  couples; //!< couples


        private:
            tableaux          ntab;
            tableaux          mtab;

        public:
            const alist        active;  //!< active species list
            tableau           &K;       //!< [N] precomputed constants
            const imatrix     Nu;       //!< [NxM] topology
            const imatrix     NuT;      //!< [MxN] Nu'
            const imatrix     NuTA;     //!< [MxN] Nu', not blocked
                                        
            tableau          &Corg;     //!< [M] internal working space
            tableau          &Ctmp;     //!< [M] internal temporary concs
            tableau          &Cend;     //!< [M] internal target concentration
            tableau          &Ctry;     //!< [M] internal trial
            tableau          &dC;       //!< [M] step

            tableau          &Xtmp;     //!< [N] temporary if R^N
            tableau          &Gamma;    //!< [N] mass actions
            tableau          &xi;       //!< [N] extents
            tableau          &_blk_;    //!< [N] data for blocked
            booltab          blocked;   //!< [N] from _blk_, transmogrified :)

            rmatrix           Psi;      //!< [NxM]    jacobian of Gamma
            rvector           Ktot;     //!< [Ntot]   all constant, singles+couples
            rvector           Xtot;     //!< [Ntot]   all Xi, to help select best reaction
            rvector           Gtot;     //!< [Ntot]   all optimized |Gamma^2|
            rmatrix           Ctot;     //!< [NtotxM] corresponding to optimized values
            rvector           rstack;   //!< [0..M] variable stack of reals

            rmatrix           Omega0;   //!< [NxN] Phi*Nu', regularized
            rmatrix           iOmega;   //!< [NxN] inv(Omega0)
            math::lu<double>  LU;       //!< inverter

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
            const lockable::scope lib_lock;
            const lockable::scope eqs_lock;

            size_t buildMatchingCouples(); //!< build all possible coupled equilibria
            double optimizeDecreaseFrom(const double G0)  throw(); //!< from G0 @Corg to Cend
            double selectDecreasedState()                 throw(); //!< select from Gtot/Ctot
            size_t computeOmegaAndGamma()                 throw(); //!< build full initial Omega and Gamma, return num running
            void   retractEquilibriumAt(const size_t ei)  throw(); //!< regularize by removing equilibrium from set
        };

    }
}

#endif


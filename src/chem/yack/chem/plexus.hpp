//! \file

#ifndef YACK_CHEM_PLEXUS_INCLUDED
#define YACK_CHEM_PLEXUS_INCLUDED 1


#include "yack/chem/library.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/data/list/meta.hpp"
#include "yack/container/matrix.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/sequence/arrays.hpp"

namespace yack
{
    namespace chemical
    {

    
        typedef matrix<int>            imatrix;  //!< alias
        typedef matrix<double>         rmatrix;  //!< alias
        typedef arrays_of<double>      tableaux; //!< alias
        typedef tableaux::array_type   tableau;  //!< alias

        //______________________________________________________________________
        //
        //
        //! plexus for ND ops
        //
        //______________________________________________________________________
        class plexus
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char  clid[];  //!< "chemical::plexus"
            static const bool &verbose; //!< on entity verbose
            static const char  vpfx[];   //!< prefix for verbosity

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit plexus(library    &lib_,
                            equilibria &eqs_);

            //! cleanup
            virtual ~plexus() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            bool solve(writable<double> &C) throw();                //!< with precomputed K
            void computeK(const double t);                          //!< per equilibrium
            void computeGamma(const readable<double> &C) throw();   //!< with precomputed K
            void computePsi(const readable<double>   &C) throw();   //!< with precomputed K
            void computeState(const readable<double> &C) throw();   //!< Gamma and Psi, with precomputed K


            //! check active concentrations
            bool are_valid(const readable<double>  &C) throw();


            //! transfer active species only
            void transfer(writable<double> &, const readable<double> & ) const throw();

            //! regularize with precomputed K
            /**
             - check Psi and best effort move, setting blocked if necessary
             */
            void regularize(writable<double> &C) throw();

            //! compute Omega0
            void   makeOmega0() throw();


            //! compute rms
            double rmsGamma(const readable<double> &C) throw();

            double operator()(const double u) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library    &lib; //!< user's lib
            const equilibria &eqs; //!< user's eqs
            const size_t      M;   //!< number of species
            const size_t      A;   //!< number of active species
            const size_t      N;   //!< number of equilibria

        private:
            tableaux          ntab;
            tableaux          mtab;

        public:
            const alist        active;  //!< active species list
            tableau           &K;       //!< [N] precomputed constants
            tableau           &Gamma;   //!< [N] mass action
            tableau           &xi;      //!< [N] current   xi
            tableau           &xs;      //!< [N] helper to sum
            tableau           &xm;      //!< [N] -Gamma
            tableau           &Xi;      //!< [N] Xi
            tableau           &Gscal;   //!< [N] scaling factor

            tableau          &Corg;    //!< [M] working space
            tableau          &Ctmp;    //!< [M] temporary C
            tableau          &Ctry;    //!< [M] trial C
            tableau          &dC;      //!< [M] delta C

            const imatrix     Nu;      //!< [NxM] topology
            const imatrix     NuT;     //!< [MxN] Nu'
            rmatrix           Psi;     //!< [NxM] jacobian
            rmatrix           Ceq;     //!< [NxM] primary eqs
            rmatrix           Omega0;  //!< [NxN]
            rmatrix           iOmega;  //!< [NxN]
            vector<bool>      blocked; //!< [N]
            vector<double>    rstack;  //!< real stack
            vector<size_t>    ustack;  //!< indx stack
            math::lu<double>  LU;      //!< [N]



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(plexus);
            const lockable::scope lib_lock;
            const lockable::scope eqs_lock;
        };

    }
}

#endif



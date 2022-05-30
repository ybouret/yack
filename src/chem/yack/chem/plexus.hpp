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
            static const char  vpfx[];  //!< prefix for verbosity

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


            //! compute rms
            double rmsGamma(const readable<double> &C) throw();

            //! aliasing rmsGamma(Corg+u*dC)
            double operator()(const double u) throw();

            //! regularize Corg
            void regularize() throw();

            //! block equilibrium
            void blockEq(const size_t ei) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library    &lib; //!< user's lib
            const equilibria &eqs; //!< user's eqs
            const library     sub; //!< hard copy
            const equilibria  pre; //!< pr eqs
            const size_t      M;   //!< number of species
            const size_t      MA;  //!< number of active species
            const size_t      MP;  //!< number of primary species
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
            tableau           &xm;      //!< [N] -Gamma/<Psi|Nu>
            tableau           &Xi;      //!< [N] Xi
            tableau           &Gs;      //!< [N] scaling factor
            tableau           &xd;      //!< [N] extra diag
            
            tableau          &Corg;    //!< [M] working space
            tableau          &Ctmp;    //!< [M] temporary C
            tableau          &Ctry;    //!< [M] trial C
            tableau          &Cend;    //!< [M] target C
            tableau          &dC;      //!< [M] delta C

            const imatrix     Nu;      //!< [NxM] topology
            const imatrix     NuT;     //!< [MxN] Nu'
            imatrix           NuAT;    //!< [NxM] active NuT
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



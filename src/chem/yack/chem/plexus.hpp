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

        //! plexus for ND ops
        class plexus
        {
        public:
            static const char  clid[];  //!< "chemical::plexus"
            static const bool &verbose; //!< on entity verbose

            //! setup
            explicit plexus(library    &lib_,
                            equilibria &eqs_);

            //! cleanup
            virtual ~plexus() throw();
            
            const library    &lib; //!< user's lib
            const equilibria &eqs; //!< user's eqs
            const size_t      M;   //!< number of species
            const size_t      A;   //!< number of active species
            const size_t      N;   //!< number of equilibria

        private:
            tableaux          ntab;
            tableaux          mtab;

        public:
            const alist       active; //!< active species list
            tableau          &K;      //!< [N] precomputed constants
            tableau          &Gamma;  //!< [N] mass action
            tableau          &Xi;     //!< [N] solving   Xi
            tableau          &xi;     //!< [N] current   xi
            tableau          &xs;     //!< [N] helper
            tableau          &xa;     //!< [N] helper

            tableau          &Ctmp;   //!< [M] temporary C
            tableau          &Ctry;   //!< [M] trial C
            tableau          &dC;     //!< [M] delta C

            const imatrix     Nu;     //!< [NxM] topology
            const imatrix     NuT;    //!< [MxN] Nu'
            rmatrix           Psi;    //!< [NxM] jacobian
            rmatrix           Ceq;    //!< [NxM] individual solution
            rmatrix           Omega0; //!< [NxN]
            rmatrix           iOmega; //!< [NxN]
            vector<double>    rstack; //!< [0..M] stack of reals
            vector<size_t>    ustack; //!< [0..M] stack of unsigned
            math::lu<double>  LU;     //!< [N]


            //! with precomputed K
            bool solve(writable<double> &C) throw();

            void computeK(const double t);                          //!< per equilibrium
            void computeGamma(const readable<double> &C) throw();   //!< with precomputed K
            void computePsi(const readable<double>   &C)   throw(); //!< with precomputed K
            void computeState(const readable<double> &C) throw();   //!< Gamma and Psi, with precomputed K

            //! compute Xi, Ceq and Psi at Ceq for each equilibrium
            double computeMissing(const readable<double>  &C)    throw();

            //! compute Omega0 after computeExcess
            void computeOmega0() throw();

            //! using LU, inv(Omega (*) xa)
            bool inverseOmega0() throw();

            void computeExtent();                                   //!< compute Omega, LU
            void correctExtent(const readable<double> &C) throw();  //!< impose primary limits
            bool computeDeltaC(const readable<double> &C) throw();  //!< deltaC and limits

            //! transfer active species only
            void transfer(writable<double> &targetC, const readable<double> &sourceC) const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(plexus);
            const lockable::scope lib_lock;
            const lockable::scope eqs_lock;
        };

    }
}

#endif



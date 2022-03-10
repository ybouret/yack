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

        typedef meta_list<const species> alist;
        typedef alist::node_type         anode;

        typedef matrix<int>          imatrix;
        typedef matrix<double>       rmatrix;
        typedef arrays_of<double>    tableaux;
        typedef tableaux::array_type tableau;

        class plexus
        {
        public:
            static const char clid[];
            
            explicit plexus(library    &lib_,
                            equilibria &eqs_);

            virtual ~plexus() throw();
            
            const library    &lib;
            const equilibria &eqs;
            const size_t      M;
            const size_t      A;
            const size_t      N;
        private:
            tableaux          ntab;
            tableaux          mtab;
        public:
            const alist       active; //!< active species list
            tableau          &K;      //!< [N] precomputed constants
            tableau          &Gamma;  //!< [N] mass action
            tableau          &Xi;     //!< [N] solving   Xi
            tableau          &Ctmp;   //!< [M] temporary C
            const imatrix     Nu;     //!< [NxM] topology
            const imatrix     NuT;    //!< [MxN] Nu'
            rmatrix           Psi;    //!< [NxM] jacobian
            rmatrix           Ceq;    //!< [NxM] individual solution
            math::lu<double>  LU;     //!< [N]



            void computeK(const double t);                          //!< per equilibrium
            void computeGamma(const readable<double> &C) throw();   //!< with precomputed K
            void computePsi(const readable<double> &C)   throw();   //!< with precomputed K
            void computeState(const readable<double> &C) throw();   //!< Gamma and Psi, with precomputed K
            void computeXi(const readable<double> &C)    throw();   //!< Xi and Ceq


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(plexus);
            const lockable::scope lib_lock;
            const lockable::scope eqs_lock;
        };

    }
}

#endif



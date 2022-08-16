
//! \file

#ifndef YACK_CHEMICAL_REACTOR_INCLUDED
#define YACK_CHEMICAL_REACTOR_INCLUDED 1

#include "yack/chem/active.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace chemical
    {

        typedef arrays_of<double>    tableaux;
        typedef tableaux::array_type tableau;
        typedef matrix<int>          imatrix;

        class reactor
        {
        public:
            virtual ~reactor() throw();

            explicit reactor(const library    &,
                             const equilibria &,
                             const double     t);

            const library    &corelib;
            const equilibria &singles;
            const active_list working;
            const size_t      M;
            const size_t      N;
            const size_t      L;
            const equilibria  couples;
            const equilibria  lattice;
            rmulops           xmul;
            raddops           xadd;

        private:
            const library     worklib;
            tableaux          mtab;
            tableaux          ntab;
            tableaux          ltab;
            
        public:
            const imatrix      Nu;  //!< [NxM] topology
            tableau           &K;  //!< [N] pre-computed K
            tableau           &Kl; //!< [L] pre-computed K


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
            const lockable::scope lockLib;
            const lockable::scope lockEqs;
        };


    }

}

#endif


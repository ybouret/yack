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


            virtual ~reactor() throw();

            explicit reactor(library    &lib_,
                             equilibria &eqs_);



            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library    &lib; //!< user's lib
            const equilibria &eqs; //!< user's eqs
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
            const imatrix     Nu;       //!< [NxM] topology
            const imatrix     NuT;      //!< [MxN] Nu'




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
            const lockable::scope lib_lock;
            const lockable::scope eqs_lock;

        };

    }
}

#endif


//! \file

#ifndef YACK_CHEMICAL_REACTOR_INCLUDED
#define YACK_CHEMICAL_REACTOR_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/container/matrix.hpp"
#include "yack/chem/active.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack {

    namespace chemical {

        class reactor
        {
        public:
            explicit reactor(library     &lib_,
                             equilibria  &eqs_,
                             const double t0);

            virtual ~reactor() throw();

            const library    &lib; //!< libray
            const equilibria &eqs; //!< singles
            const equilibria  all; //!< lattice
            const active_list act; //!< active species
            const size_t      M;
            const size_t      N;
            const matrix<int> Nu; //!< global topology

        private:
            const lockable::scope libLock;
            const lockable::scope eqsLock;

            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
        };

    }
}

#endif


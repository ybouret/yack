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

        //______________________________________________________________________
        //
        //
        //! reactor
        //
        //______________________________________________________________________
        class reactor
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char * const clid;     //!< "[reactor]"
            static bool              &verbose;  //!< global

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! full setup
            explicit reactor(library     &lib_,
                             equilibria  &eqs_,
                             const double t0);

            //! cleanup
            virtual ~reactor() throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library    &lib; //!< library
            const equilibria &eqs; //!< singles
            const active_list act; //!< active species
            const size_t      M;   //!< total species
            const size_t      N;   //!< number of single
            const matrix<int> Nu;  //!< global topology



        private:
            const lockable::scope libLock;
            const lockable::scope eqsLock;

            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
        };

    }
}

#endif


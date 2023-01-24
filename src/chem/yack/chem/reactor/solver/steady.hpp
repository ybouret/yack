
//! \file

#ifndef YACK_CHEMICAL_STEADY_INCLUDED
#define YACK_CHEMICAL_STEADY_INCLUDED 1

#include "yack/chem/reactor.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/data/dinky/solo-repo.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //
        //! scene of species and equilibria
        //
        //______________________________________________________________________
        class steady
        {
        public:
            explicit steady(const reactor    &cs_,
                            writable<double> &K_,
                            const double      t0);

            virtual ~steady() throw();

            const reactor    &cs;   //!< persistent reactor
            writable<double> &K;    //!< persistent array of constants
            matrix<double>    Cs;   //!< [LxM]

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(steady);

        };

    }

}

#endif


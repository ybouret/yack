
//! \file

#ifndef YACK_CHEMICAL_BOUNDARIES_INCLUDED
#define YACK_CHEMICAL_BOUNDARIES_INCLUDED 1

#include "yack/chem/reactor/balance/frontiers.hpp"

namespace yack {

    namespace chemical
    {

        class boundaries
        {
        public:
            boundaries(const size_t m);
            ~boundaries() throw();

            frontiers limiting; //!< from positive concentration
            frontiers amending; //!< from negative concentration

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(boundaries);
        };


    }

}

#endif


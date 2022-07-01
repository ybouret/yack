
//! \file

#ifndef YACK_CHEM_MIX_COUPLES_INCLUDED
#define YACK_CHEM_MIX_COUPLES_INCLUDED 1

#include "yack/chem/equilibria.hpp"

namespace yack
{
    namespace chemical
    {

        struct coupling
        {
            //! ...
            /**
             \param couples target couples
             \param singles source singles
             \param Nu [NxM] topology matrix
             \param K  [N]   source pre-computed constants
             */
            static size_t build(equilibria             &couples,
                                const equilibria       &singles,
                                const matrix<int>      &Nu,
                                const readable<double> &K,
                                const library          &sub);
        };

    }

}

#endif

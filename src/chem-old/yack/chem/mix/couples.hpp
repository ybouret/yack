
//! \file

#ifndef YACK_CHEM_MIX_COUPLES_INCLUDED
#define YACK_CHEM_MIX_COUPLES_INCLUDED 1

#include "yack/chem/equilibria.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! coupling algorithm
        //
        //______________________________________________________________________
        struct coupling
        {
            //! build all the possible couples
            /**
             \param couples target couples
             \param singles source singles
             \param Nu [NxM] topology matrix
             \param K  [N]   source pre-computed constants
             \param L  [M]   library of duplicated species
             */
            static size_t build(equilibria             &couples,
                                const equilibria       &singles,
                                const matrix<int>      &Nu,
                                const readable<double> &K,
                                const library          &L);
        };

    }

}

#endif


//! \file

#ifndef YACK_CHEM_HIDDEN_INCLUDED
#define YACK_CHEM_HIDDEN_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace chemical
    {

        struct hidden
        {
            static double diff(const double lhs, const double rhs) throw();
        };

    }

}

#endif

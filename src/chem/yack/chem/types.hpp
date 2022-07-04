//! \file

#ifndef YACK_CHEM_TYPES_INCLUDED
#define YACK_CHEM_TYPES_INCLUDED 1

#include "yack/sequence/arrays.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace chemical
    {


        typedef matrix<int>            imatrix;  //!< alias
        typedef matrix<double>         rmatrix;  //!< alias
        typedef vector<double>         rvector;  //!< alias
        typedef arrays_of<double>      tableaux; //!< alias
        typedef tableaux::array_type   tableau;  //!< alias
        typedef thin_array<bool>       booltab;  //!< alias
        typedef math::lu<double>       rsolver;  //!< alias

    }

}

#endif

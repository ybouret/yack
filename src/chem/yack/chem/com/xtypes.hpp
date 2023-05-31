

#ifndef YACK_CHEM_XTYPES_INCLUDED
#define YACK_CHEM_XTYPES_INCLUDED 1

#include "yack/ipso/add.hpp"
#include "yack/data/dinky/core-list.hpp"

namespace yack {

    namespace Chemical
    {

        typedef ipso::add<double>  Adder; //!< alias
        typedef Adder::inside_type XReal; //!< alias
        
        //! conversion from double to extended
        inline XReal  Double2XReal(const double x) { return ipso::inside<double>::send(x); }

        //! conversion from extended to double
        inline double XReal2Double(const XReal &x) { return ipso::inside<double>::recv(x); }

        typedef core_list<XReal>         XRealCoreList; //!< alias
        typedef XRealCoreList::node_type XRealNode; //!< alias
        

    }

}

#endif




#ifndef YACK_CHEM_XTYPES_INCLUDED
#define YACK_CHEM_XTYPES_INCLUDED 1

#include "yack/ipso/add.hpp"
#include "yack/data/dinky/core-list.hpp"

namespace yack {

    namespace Chemical
    {

        struct Extended
        {
            typedef extended<double>  Real;
            typedef ipso::add<double> Adder;

            //! conversion from double to extended
            static inline Real   Send(const double x) { return ipso::inside<double>::send(x); }

            //! conversion from extended to double
            static inline double Recv(const Real &x) { return ipso::inside<double>::recv(x); }


            typedef core_list<Real>     CoreList;
            typedef CoreList::node_type Node;

        };

        typedef Extended::Real    XReal; //!< alias
        
        
        

    }

}

#endif


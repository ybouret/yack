
//! \file

#ifndef YACK_AQUEOUS_COLL_CHART_INCLUDED
#define YACK_AQUEOUS_COLL_CHART_INCLUDED 1

#include "yack/aqueous/eqs/frontiers.hpp"
#include "yack/aqueous/realm/coll/caches.hpp"
#include "yack/aqueous/eqs/components.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace collecting
        {
            //__________________________________________________________________
            //
            //
            //! chart of frontiers for reactants and products
            //
            //__________________________________________________________________
            class chart
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit chart(const caches &) noexcept; //!< setup
                virtual ~chart()               noexcept; //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void operator()(const components       &,
                                const readable<double> &);
                YACK_PROTO_OSTREAM(chart);
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                frontiers reac; //!< frontiers for reactant
                frontiers prod; //!< frontiers for products
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(chart);
            };
        }
    }

}

#endif

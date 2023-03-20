
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

                //! detect all status
                /**
                 \param E components with its reac and its prod
                 \param C global concentrations
                 \param R global regular flags
                 */
                bool settle(const components       &E,
                            const readable<double> &C,
                            const readable<bool>   &R);

                //! display result
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

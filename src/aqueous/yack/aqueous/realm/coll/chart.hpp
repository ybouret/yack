
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

                //! out of range type
                enum oor_type
                {
                    oor_none, //!< no oor
                    oor_reac, //!< reactants are oor
                    oor_prod, //!< products are oor
                    oor_both  //!< both are oor
                };


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
                oor_type settle(const components       &E,
                                const readable<double> &C,
                                const readable<bool>   &R);


                //! adjust invalid products with limiting reactant
                /**
                 - called on oor_prod
                 - return true if products are positive again
                 */
                bool     adjust_prod(const xmlog &);

                //! adjust invalid reactants with limiting products
                /**
                 - called on oor_reac
                 - return true if products are positive again
                 */
                bool     adjust_reac(const xmlog &);

                //! display 
                YACK_PROTO_OSTREAM(chart);

                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                frontiers reac; //!< frontiers for reactant
                frontiers prod; //!< frontiers for products
                zlimit    corr; //!< correction if possible
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(chart);

            };
        }
    }

}

#endif

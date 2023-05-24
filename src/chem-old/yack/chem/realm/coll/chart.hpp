
//! \file

#ifndef YACK_CHEMICAL_COLL_CHART_INCLUDED
#define YACK_CHEMICAL_COLL_CHART_INCLUDED 1

#include "yack/chem/eqs/frontiers.hpp"
#include "yack/chem/realm/coll/caches.hpp"
#include "yack/chem/eqs/components.hpp"

namespace yack
{
    namespace chemical
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
                // types and definitions
                //______________________________________________________________

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
                 - called on oor.prod with limiting reac
                 - return status
                 */
                shift_status adjust_prod(const xmlog &);

                //! adjust invalid reactants with limiting products
                /**
                 - called on oor.reac with limiting prod
                 - return status, changed sign of corrr
                 */
                shift_status adjust_reac(const xmlog &);

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

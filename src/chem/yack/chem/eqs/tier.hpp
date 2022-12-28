//! \file

#ifndef YACK_CHEMICAL_EQ_TIER_INCLUDED
#define YACK_CHEMICAL_EQ_TIER_INCLUDED 1

#include "yack/chem/eqs/team.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! for classification of equilibria
        //
        //______________________________________________________________________
        class eq_tier : public object
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<eq_tier> pointer; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit eq_tier() throw(); //!< setup empty
            virtual ~eq_tier() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const eq_team roaming; //!< roaming equilibiria
            const eq_team bounded; //!< bounded equilibria

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_tier);
        };

    }

}

#endif


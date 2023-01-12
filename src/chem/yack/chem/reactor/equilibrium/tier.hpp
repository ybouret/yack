//! \file

#ifndef YACK_CHEMICAL_EQ_TIER_INCLUDED
#define YACK_CHEMICAL_EQ_TIER_INCLUDED 1

#include "yack/chem/reactor/equilibrium/group.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //
        //! making tiers depending on equilibria chracterictics
        //
        //______________________________________________________________________
        class eq_tier : public object
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<const eq_tier> ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit eq_tier();          //!< setup
            virtual ~eq_tier() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const auto_ptr<eq_group> bounded; //!< at least one conserved species in both sides
            const auto_ptr<eq_group> roaming; //!< prod only or reac only
            const auto_ptr<eq_group> special; //!< all reac or all prod, or both are unbounded

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_tier);
        };

    }
}

#endif


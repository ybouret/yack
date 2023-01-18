//! \file

#ifndef YACK_CHEMICAL_EQ_TIER_INCLUDED
#define YACK_CHEMICAL_EQ_TIER_INCLUDED 1

#include "yack/chem/reactor/equilibrium/group.hpp"
#include "yack/chem/reactor/equilibrium/repo.hpp"

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
            // method
            //__________________________________________________________________
            bool dispatch(const gnode *gn); //!< according to reac/prod, return true if new roaming

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const eq_group::ptr      delimited; //!< at least one conserved species in both sides
            const eq_group::ptr      reac_only; //!< only reactants
            const eq_group::ptr      prod_only; //!< only products
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_tier);
        };

    }
}

#endif


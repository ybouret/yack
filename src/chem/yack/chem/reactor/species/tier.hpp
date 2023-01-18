//! \file

#ifndef YACK_CHEMICAL_SP_TIER_INCLUDED
#define YACK_CHEMICAL_SP_TIER_INCLUDED 1

#include "yack/chem/active.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //
        // sub-list of existing species for a cluster
        //
        //______________________________________________________________________
        typedef core_repo<const anode> sp_group; //!< alias
        typedef sp_group::node_type    sp_gnode; //!< alias

        //______________________________________________________________________
        //
        //
        //! making tiers of species
        //
        //______________________________________________________________________
        class sp_tier : public object
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<const sp_tier> ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit sp_tier();          //!< setup
            virtual ~sp_tier() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void display(const xmlog &xml, const char * const pfx) const;


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const auto_ptr<sp_group> conserved; //!< regular species
            const auto_ptr<sp_group> unbounded; //!< in a roaming/special equilibrium

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(sp_tier);
        };


        

    }

}
#endif

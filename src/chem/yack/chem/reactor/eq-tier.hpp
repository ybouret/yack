//! \file

#ifndef YACK_CHEMICAL_EQ_TIER_INCLUDED
#define YACK_CHEMICAL_EQ_TIER_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/chem/sub-list.hpp"
#include "yack/ptr/auto.hpp"

namespace yack {

    namespace chemical {


        //______________________________________________________________________
        //
        //
        // sub-list of existing equilibria for a cluster
        //
        //______________________________________________________________________
        typedef sub_node<equilibrium>    gnode; //!< alias
        typedef sub_list<equilibrium>    glist; //!< alias
        
        //______________________________________________________________________
        //
        //
        // repository based on sub-lists
        //
        //______________________________________________________________________
        typedef core_repo<const gnode>  eq_group_; //!< alias
        typedef eq_group_::node_type    eq_gnode;  //!< alias

        //______________________________________________________________________
        //
        //
        //! group of equilibria from a sub-list
        //
        //______________________________________________________________________
        class eq_group : public object, public eq_group_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit eq_group() throw();
            virtual ~eq_group() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! specific display
            friend std::ostream & operator<<(std::ostream &, const eq_group &);
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_group) throw();
        };

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

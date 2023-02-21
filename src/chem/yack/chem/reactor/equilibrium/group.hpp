
#ifndef YACK_CHEMICAL_EQ_GROUP_INCLUDED
#define YACK_CHEMICAL_EQ_GROUP_INCLUDED 1

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
            // types
            //__________________________________________________________________
            typedef auto_ptr<const eq_group> ptr; //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit eq_group() noexcept;
            virtual ~eq_group() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            YACK_PROTO_OSTREAM(eq_group); //!< specific display
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_group) noexcept;
        };
    }

}

#endif


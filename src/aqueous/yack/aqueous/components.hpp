
//! \file

#ifndef YACK_AQUEOUS_COMPONENTS_INCLUDED
#define YACK_AQUEOUS_COMPONENTS_INCLUDED 1

#include "yack/aqueous/component.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        // types for components
        //
        //______________________________________________________________________
        typedef suffix_set<string,component::ptr> components_db; //!< databse of components
        typedef components_db::knot_type          cnode;         //!< alias

        //______________________________________________________________________
        //
        //
        //! components are a database of components
        //
        //______________________________________________________________________
        class components : public large_object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char * const clid; //!< "components"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit components() noexcept; //!< setup empty
            virtual ~components() noexcept; //!< cleanup
            components(const components &); //!< hard copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const components_db::tree_type * operator->() const noexcept;      //!< get internal tree
            const components_db::tree_type & operator*()  const noexcept;      //!< get internal tree

            void operator()(const int nu, const species &sp); //!< declare a new components
            YACK_PROTO_OSTREAM(components);                   //!< display
            string to_string() const;                         //!< stringify
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const actors reac; //!< list of reactants
            const actors prod; //!< list of products
            
        private:
            YACK_DISABLE_ASSIGN(components);
            components_db cdb;
        };

    }

}

#endif

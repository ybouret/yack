
//! \file

#ifndef YACK_AQUEOUS_COMPONENTS_INCLUDED
#define YACK_AQUEOUS_COMPONENTS_INCLUDED 1

#include "yack/aqueous/component.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace aqueous
    {

        typedef suffix_set<string,component::ptr> components_db; //!< databse of components
        typedef components_db::knot_type          cnode;         //!< alias

        class components : public large_object, public counted
        {
        public:
            static const char * const clid;
            
            explicit components() noexcept;
            virtual ~components() noexcept;
            components(const components &);

            const components_db::tree_type * operator->() const noexcept;      //!< get internal tree
            const components_db::tree_type & operator*()  const noexcept;      //!< get internal tree

            void operator()(const int nu, const species &sp);
            YACK_PROTO_OSTREAM(components);

            const actors reac;
            const actors prod;
            
        private:
            YACK_DISABLE_ASSIGN(components);
            components_db cdb;
        };

    }

}

#endif

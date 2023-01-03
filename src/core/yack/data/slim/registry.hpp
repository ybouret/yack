//! \file

#ifndef YACK_DATA_SLIM_REGISTRY_INCLUDED
#define YACK_DATA_SLIM_REGISTRY_INCLUDED 1

#include "yack/data/slim/catalog.hpp"

namespace yack
{
    
    template <typename T, typename ZPOOL>
    class slim_registry : public slim_catalog< slim_node<T>, ZPOOL>
    {
    public:
        typedef slim_node<T>                  node_type;
        typedef slim_catalog<node_type,ZPOOL> self_type;
        
        inline explicit slim_registry() throw() : self_type() {}
        inline explicit slim_registry(const ZPOOL &user) throw() : self_type(user) {}
        inline virtual ~slim_registry() throw() {}
        inline slim_registry(const slim_registry &other) : self_type(other) {}

        //! sort function
        template <typename FUNC> inline
        void sort_with( FUNC &func )
        {
            kernel::primary_sort_callback<node_type,FUNC> proc = { func };
            merge_list_of<node_type>::sort(*this,proc);
        }
        
    private:
        YACK_DISABLE_ASSIGN(slim_registry);
    };
    
}

#endif


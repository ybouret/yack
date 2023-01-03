//! \file

#ifndef YACK_DATA_SLIM_MANIFEST_INCLUDED
#define YACK_DATA_SLIM_MANIFEST_INCLUDED 1

#include "yack/data/slim/catalog.hpp"
#include "yack/data/slim/ptr.hpp"

namespace yack
{
    
    template <typename T, typename ZPOOL>
    class slim_manifest : public slim_catalog< slim_node< slim_ptr<T> >, ZPOOL>
    {
    public:
        typedef slim_ptr<T>                   data_type;
        typedef slim_node<data_type>          node_type;
        typedef slim_catalog<node_type,ZPOOL> self_type;
        
        inline explicit slim_manifest() throw() : self_type() {}
        inline explicit slim_manifest(const ZPOOL &user) throw() : self_type(user) {}
        inline virtual ~slim_manifest() throw() {}
        inline slim_manifest(const slim_manifest &other) : self_type(other) {}
        
        
        //! sorting by arguments
        template <typename FUNC> inline
        void sort_with( FUNC &func )
        {
            kernel::replica_sort_callback<node_type,FUNC> proc = { func };
            merge_list_of<node_type>::sort(*this,proc);
        }
        
    private:
        YACK_DISABLE_ASSIGN(slim_manifest);
    };
    
}

#endif


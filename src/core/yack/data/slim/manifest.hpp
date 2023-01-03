//! \file

#ifndef YACK_DATA_SLIM_MANIFEST_INCLUDED
#define YACK_DATA_SLIM_MANIFEST_INCLUDED 1

#include "yack/data/slim/catalog.hpp"
#include "yack/data/slim/ptr.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! manifest = catalog of pointers
    //
    //__________________________________________________________________________
    template <typename T, typename ZPOOL>
    class slim_manifest : public slim_catalog< slim_node< slim_ptr<T> >, ZPOOL>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);                         //!< alias
        typedef slim_ptr<T>                   data_type; //!< alias
        typedef slim_node<data_type>          node_type; //!< alias
        typedef slim_catalog<node_type,ZPOOL> self_type; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
      
        //! setup for slim_hook
        inline explicit slim_manifest() throw() : self_type() {}
        
        //! setup for slim_bank::pointer
        inline explicit slim_manifest(const ZPOOL &user) throw() : self_type(user) {}
        
        //! cleanup
        inline virtual ~slim_manifest() throw() {}
        
        //! delegates copy
        inline slim_manifest(const slim_manifest &other) : self_type(other) {}
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! sorting by arguments
        template <typename FUNC> inline
        void sort_with( FUNC &func )
        {
            kernel::replica_sort_callback<node_type,FUNC> proc = { func };
            merge_list_of<node_type>::sort(*this,proc);
        }
        
        //! override push_back
        slim_manifest & operator<<(type &obj)
        {
            this->push_back( this->create( &obj ) );
            return *this;
        }
        
        //! override push_front
        slim_manifest & operator>>(type &obj)
        {
            this->push_front( this->create( &obj ) );
            return *this;
        }
        
        
        
    private:
        YACK_DISABLE_ASSIGN(slim_manifest);
    };
    
}

#endif


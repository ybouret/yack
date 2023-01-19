

//! \file

#ifndef YACK_DINKY_MANIFEST_INCLUDED
#define YACK_DINKY_MANIFEST_INCLUDED 1

#include "yack/data/dinky/node.hpp"
#include "yack/data/dinky/ptr.hpp"
#include "yack/data/dinky/root.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    
    
#define YACK_DINKY_MANIFEST_NODE dinky_node< dinky_ptr<T> > //!< helper for syntax
    
    //__________________________________________________________________________
    //
    //
    //! manifest of addresses
    //
    //__________________________________________________________________________
    template <typename T, template <typename> class ZPOOL>
    class  dinky_manifest :
    public dinky_root< YACK_DINKY_MANIFEST_NODE, typename ZPOOL< YACK_DINKY_MANIFEST_NODE >::proxy >
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef dinky_ptr<T>                     data_type;  //!< alias
        typedef dinky_node<data_type>            node_type;  //!< alias
        typedef ZPOOL<node_type>                 zpool_type; //!< alias
        typedef typename zpool_type::proxy       proxy_type; //!< alias
        typedef dinky_root<node_type,proxy_type> root_type;  //!< alias
        typedef typename root_type::list_type    list_type;  //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit dinky_manifest() throw() : root_type()                           {} //!< setup empty
        inline explicit dinky_manifest(const proxy_type &user) throw() : root_type(user) {} //!< setup with cache
        inline explicit dinky_manifest(const dinky_manifest &_) : root_type(_)           {} //!< copy
        inline virtual ~dinky_manifest() throw()                                         {} //!< cleanup
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! push back
        template <typename U> inline
        dinky_manifest & operator<<(const U &u) {
            this->annex( (U*)&u );
            return *this;
        }
        
        //! push front
        template <typename U> inline
        dinky_manifest & operator>>(const U &u) {
            this->shove( (U*)&u );
            return *this;
        }
        
        //! sort with function action on node content
        template <typename FUNC> inline
        void sort_with( FUNC &func )
        {
            kernel::replica_sort_callback<node_type,FUNC> cb = { func };
            merge_list_of<node_type>::sort(*this,cb);
        }


        template <typename U> inline
        node_type *contains(const U &u)  throw()
        {
            for(node_type *node=this->head;node;node=node->next)
            {
                if( & (***node) == &u ) return node;
            }
            return NULL;
        }

        template <typename U> inline
        bool contains(const U &u) const throw()
        {
            for(const node_type *node=this->head;node;node=node->next)
            {
                if( & (***node) == &u ) return true;
            }
            return false;
        }

    private:
        YACK_DISABLE_ASSIGN(dinky_manifest);
    };
}

#endif


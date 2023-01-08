
//! \file

#ifndef YACK_DINKY_REGISTRY_INCLUDED
#define YACK_DINKY_REGISTRY_INCLUDED 1

#include "yack/data/dinky/node.hpp"
#include "yack/data/dinky/root.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    
    template <typename T, template <typename> class ZPOOL>
    class  dinky_registry :
    public dinky_root<dinky_node<T>, typename ZPOOL< dinky_node<T> >::proxy >
    {
    public:
        typedef dinky_node<T>                    node_type;
        typedef ZPOOL<node_type>                 zpool_type;
        typedef typename zpool_type::proxy       proxy_type;
        typedef dinky_root<node_type,proxy_type> root_type;
        typedef typename root_type::list_type    list_type;

        inline explicit dinky_registry() throw() : root_type() {}
        inline explicit dinky_registry(const proxy_type &user) throw() : root_type(user) {}
        inline virtual ~dinky_registry() throw() {}
        
        template <typename U> inline
        dinky_registry & operator<<(const U &u) {
            this->push_back( this->cache->create(u) );
            return *this;
        }
        
        template <typename U> inline
        dinky_registry & operator>>(const U &u) {
            this->push_front( this->cache->create(u) );
            return *this;
        }
        
        template <typename FUNC> inline
        void sort_with( FUNC &func )
        {
            kernel::primary_sort_callback<node_type,FUNC> cb = { func };
            merge_list_of<node_type>::sort(*this,cb);
        }
        
    private:
        YACK_DISABLE_ASSIGN(dinky_registry);
    };
    
}

#endif


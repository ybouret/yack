

//! \file

#ifndef YACK_DINKY_MANIFEST_INCLUDED
#define YACK_DINKY_MANIFEST_INCLUDED 1

#include "yack/data/dinky/node.hpp"
#include "yack/data/dinky/ptr.hpp"
#include "yack/data/dinky/root.hpp"

namespace yack
{
    
#define YACK_DINKY_MANIFEST_NODE dinky_node< dinky_ptr<T> >
    
    template <typename T, template <typename> class ZPOOL>
    class  dinky_manifest :
    public dinky_root< YACK_DINKY_MANIFEST_NODE, typename ZPOOL< YACK_DINKY_MANIFEST_NODE >::proxy >
    {
    public:
        typedef dinky_ptr<T>                     data_type;
        typedef dinky_node<data_type>            node_type;
        typedef ZPOOL<node_type>                 zpool_type;
        typedef typename zpool_type::proxy       proxy_type;
        typedef dinky_root<node_type,proxy_type> root_type;
        
        inline explicit dinky_manifest() throw() : root_type() {}
        inline explicit dinky_manifest(const proxy_type &user) throw() : root_type(user) {}
        inline virtual ~dinky_manifest() throw() {}
        
        
    private:
        YACK_DISABLE_ASSIGN(dinky_manifest);
    };
}

#endif


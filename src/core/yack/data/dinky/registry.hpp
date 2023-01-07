
//! \file

#ifndef YACK_DINKY_REGISTRY_INCLUDED
#define YACK_DINKY_REGISTRY_INCLUDED 1

#include "yack/data/dinky/node.hpp"
#include "yack/data/dinky/root.hpp"

namespace yack
{
    
    template <typename T, typename ZPROXY>
    class dinky_registry : public dinky_root<dinky_node<T>,ZPROXY>
    {
    public:
        typedef dinky_node<T> node_type;
        typedef dinky_root<node_type,ZPROXY> root_type;
        
        inline explicit dinky_registry() throw() : root_type() {}
        inline explicit dinky_registry(const ZPROXY &user) throw() : root_type(user) {}
        inline virtual ~dinky_registry() throw() {}
        
        
    private:
        YACK_DISABLE_ASSIGN(dinky_registry);
    };
    
}

#endif


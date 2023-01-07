

//! \file

#ifndef YACK_DINKY_MANIFEST_INCLUDED
#define YACK_DINKY_MANIFEST_INCLUDED 1

#include "yack/data/dinky/node.hpp"
#include "yack/data/dinky/ptr.hpp"
#include "yack/data/dinky/root.hpp"

namespace yack
{
    
    template <typename T, typename ZPROXY>
    class dinky_manifest : public dinky_root<dinky_node< dinky_ptr<T> >,ZPROXY>
    {
    public:
        typedef dinky_ptr<T>          data_type;
        typedef dinky_node<data_type> node_type;
        
        
    private:
        YACK_DISABLE_ASSIGN(dinky_manifest);
    };
}

#endif


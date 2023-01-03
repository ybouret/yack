//! \file

#ifndef YACK_DATA_SLIM_PARA_LIST_INCLUDED
#define YACK_DATA_SLIM_PARA_LIST_INCLUDED 1

#include "yack/data/slim/node.hpp"
#include "yack/data/slim/ptr.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    

    template <typename T>
    class para_list : public cxx_list_of< slim_node< slim_ptr<T> > >
    {
    public:
        YACK_DECL_ARGS_(T,type);
        typedef slim_ptr<T>            hook_type;
        typedef slim_node<hook_type>   node_type;
        typedef cxx_list_of<node_type> list_type;

        inline explicit para_list() throw() : list_type() {}
        inline virtual ~para_list() throw() {}
        inline para_list(const para_list &_) : list_type(_) {}



    private:
        YACK_DISABLE_ASSIGN(para_list);
    };

}

#endif


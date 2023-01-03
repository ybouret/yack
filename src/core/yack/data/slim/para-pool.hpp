
//! \file

#ifndef YACK_DATA_SLIM_PARA_POOL_INCLUDED
#define YACK_DATA_SLIM_PARA_POOL_INCLUDED 1

#include "yack/data/slim/node.hpp"
#include "yack/data/slim/ptr.hpp"
#include "yack/container/releasable.hpp"
#include "yack/data/pool.hpp"

namespace yack
{

    template <typename T>
    class para_pool : public pool_of< slim_node< slim_ptr<T> > >, public releasable
    {
    public:
        YACK_DECL_ARGS_(T,type);
        typedef slim_ptr<T>            hook_type;
        typedef slim_node<hook_type>   node_type;
        typedef pool_of<node_type>     pool_type;

        inline explicit para_pool() throw() : pool_type() {}
        inline virtual ~para_pool() throw() { release(); }


        inline virtual void release() throw() {
            while(this->size) node_type::zrelease(this->query());
        }

        inline void reserve(size_t n) throw() {
            while(n-- > 0) this->store( node_type::zcreate() );
        }

        inline void zstore(node_type *alive) throw() {
            this->store( node_type::zombify(alive) );
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(para_pool);
    };


}

#endif

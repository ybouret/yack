#include "yack/apex/north.hpp"
#include "yack/system/imported.hpp"
#include "yack/memory/allocator/global.hpp"

namespace yack
{

    namespace north
    {

        size_t constellation:: checked_dimension(const size_t d)
        {
            if(d<=0) throw imported::exception("north","null dimension");
            return d;
        }

#if 0
        void  *constellation:: map_linear_memory(memory::embed emb[],const size_t num, size_t &bs)
        {
            //static memory::allocator &mem = memory::dyadic::instance();
            static memory::allocator &mem = memory::global::instance();

            return memory::embed::zalloc(emb,num,mem,bs);
        }
#endif
    }

}

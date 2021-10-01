#include "yack/type/out-of-reach.hpp"
#include <cstring>

namespace yack
{

    void * out_of_reach::address(void *addr) throw()
    {
        return addr;
    }

    const void * out_of_reach::address(const void *addr) throw()
    {
        return addr;
    }
    
    void out_of_reach::zset(void *addr, const size_t size) throw()
    {
        assert(!(NULL==addr && size>0));
        memset(addr,0,size);
    }

    void out_of_reach::copy(void *target, const void *source, const size_t size) throw()
    {
        assert(!(NULL==target && size>0));
        assert(!(NULL==source && size>0));
        memcpy(target,source,size);
    }

    void out_of_reach::move(void *target, const void *source, const size_t size) throw()
    {
        assert(!(NULL==target && size>0));
        assert(!(NULL==source && size>0));
        memmove(target,source,size);
    }
    
}

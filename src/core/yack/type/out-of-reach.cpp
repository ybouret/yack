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
    
    void *out_of_reach::zset(void *addr, const size_t size) throw()
    {
        assert(!(NULL==addr && size>0));
        memset(addr,0,size);
        return addr;
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

    unit_t out_of_reach:: diff(const void *a, const void *b) throw()
    {
        const char *B = static_cast<const char *>(b);
        const char *A = static_cast<const char *>(a);
        return static_cast<unit_t>(B-A);
    }

    bool out_of_reach::is0(const void *addr, const size_t size) throw()
    {
        assert(!(NULL==addr && size>0));
        const uint8_t *p = static_cast<const uint8_t *>(addr);
        for(size_t i=size;i>0;--i,++p)
        {
            if(0 != *p) return false;
        }
        return true;
    }

    void out_of_reach::swap(void *target, void *source, size_t size) throw()
    {
        assert(!(NULL==target && size>0));
        assert(!(NULL==source && size>0));
        uint8_t *p = static_cast<uint8_t *>(source);
        uint8_t *q = static_cast<uint8_t *>(target);
        while(size-- > 0)
        {
            cswap(*(p++),*(q++));
        }
    }
}

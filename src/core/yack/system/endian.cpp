#include "yack/system/endian.hpp"
#include "yack/setup.hpp"
#include "yack/type/hexa.hpp"
//#include <iostream>

namespace yack {

    bool endian:: BE() noexcept
    {
        static const union {
            uint32_t i;
            uint8_t  c[4];
        } alias = {0x01020304};
        static const bool result = (alias.c[0] == 1);
        return result;
    }

    bool endian:: LE() noexcept
    {
        return !BE();
    }

    namespace
    {
        static inline void swap_block(void *addr, const size_t size) noexcept
        {
            assert(addr);
            assert(size);
            uint8_t *lower = static_cast<uint8_t *>(addr);
            uint8_t *upper = static_cast<uint8_t *>(addr) + size;
            size_t   count = size;

            for(count>>=1;count>0;--count)
            {
                cswap(*(lower++),*(--upper));
            }
            
        }

        static inline void keep_block(void *,size_t) noexcept
        {

        }

    }

    endian::swap_proc endian:: BEswap() noexcept
    {
        static const swap_proc proc = BE() ? keep_block : swap_block;
        return proc;
    }

    void       *endian:: BEaddr(void *addr,size_t size) noexcept
    {
        static const swap_proc proc = BEswap();
        proc(addr,size);
        return addr;
    }


}

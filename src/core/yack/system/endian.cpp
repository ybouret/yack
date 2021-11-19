#include "yack/system/endian.hpp"
#include "yack/setup.hpp"

namespace yack {

    bool endianness:: BE() throw()
    {
        static const union {
            uint32_t i;
            uint8_t  c[4];
        } alias = {0x01020304};
        static const bool result = (alias.c[0] == 1);
        return result;
    }

    bool endianness:: LE() throw()
    {
        return !BE();
    }

    namespace
    {
        static inline void swap_block(void *addr, size_t size) throw()
        {
            assert(addr);
            assert(size);
            uint8_t *lower = static_cast<uint8_t *>(addr);
            uint8_t *upper = static_cast<uint8_t *>(addr) + size;
            for(size>>=1;size>0;--size)
            {
                cswap(*(lower++),*(--upper));
            }
        }

        static inline void keep_block(void *,size_t) throw()
        {

        }

    }

    endianness::swap_proc endianness::BEswap() throw()
    {
        static const swap_proc proc = BE() ? keep_block : swap_block;
        return proc;
    }

}

#include "yack/system/endian.hpp"
#include "yack/setup.hpp"
#include "yack/type/hexa.hpp"
//#include <iostream>

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
        static inline void swap_block(void *addr, const size_t size) throw()
        {
            assert(addr);
            assert(size);
            uint8_t *lower = static_cast<uint8_t *>(addr);
            uint8_t *upper = static_cast<uint8_t *>(addr) + size;
            size_t   count = size;

#if 0
            {
                std::cerr << "BEswap" << size * 8 << " : ";
                for(size_t i=0;i<size;++i)
                {
                    std::cerr << hexa::lowercase_text[ lower[i] ];
                }
                std::cerr << " -> ";
            }
#endif

            for(count>>=1;count>0;--count)
            {
                cswap(*(lower++),*(--upper));
            }

#if 0
            {
                lower = static_cast<uint8_t *>(addr);
                for(size_t i=0;i<size;++i)
                {
                    std::cerr << hexa::lowercase_text[ lower[i] ];
                }
                std::cerr << std::endl;
            }
#endif

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

    void       *endianness:: BEaddr(void *addr,size_t size) throw()
    {
        static const swap_proc proc = BEswap();
        proc(addr,size);
        return addr;
    }


}

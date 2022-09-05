#include "yack/info/compression/alphabet.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace information
    {

        namespace codec
        {


            const ESC_NYT_t ESC_NYT = {};
            const ESC_END_t ESC_END = {};

            void glyph_ops:: display_raw(std::ostream &os, const void *addr, const size_t size)
            {
                assert(addr!=NULL);
                switch(size)
                {
                    case 1:
                        os << ios::ascii::hybrid[ *static_cast<const uint8_t*>(addr) ];
                        break;

                    case 2: {
                        const uint16_t w  = *static_cast<const uint16_t *>(addr);
                        os << ios::ascii::hybrid[ uint8_t(w>>8) ] << ios::ascii::hybrid[ uint8_t(w&0xff) ];
                    } break;

                }
            }

            memory::allocator & glyph_ops:: memmgr()
            {
                static memory::allocator &mgr = memory::dyadic::instance();
                return mgr;
            }

            void glyph_ops:: release(void * &block_addr, size_t &block_size) throw()
            {
                static memory::allocator &mgr = memory::dyadic::location();
                mgr.release(block_addr,block_size);
            }


        }

    }

}

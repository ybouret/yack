#include "yack/info/compression/alphabet.hpp"
#include "yack/ios/ascii/hybrid.hpp"

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

        }

    }

}

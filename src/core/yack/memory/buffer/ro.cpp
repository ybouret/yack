
#include "yack/memory/buffer/ro.hpp"
#include "yack/type/hexa.hpp"
#include <iostream>

namespace yack
{

    namespace memory
    {
        ro_buffer:: ~ro_buffer() throw()
        {
        }

        ro_buffer:: ro_buffer() throw()
        {
        }

        std::ostream &ro_buffer:: display_hexa(std::ostream &os) const
        {
            const uint8_t *p = static_cast<const uint8_t *>(ro_addr());
            for(size_t i=measure();i>0;--i)
            {
                os << hexa::uppercase_text[*(p++)];
            }
            return os;
        }
    }

}



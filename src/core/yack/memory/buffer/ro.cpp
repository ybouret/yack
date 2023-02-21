
#include "yack/memory/buffer/ro.hpp"
#include "yack/type/hexa.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include <iostream>

namespace yack
{

    namespace memory
    {
        ro_buffer:: ~ro_buffer() noexcept
        {
        }

        ro_buffer:: ro_buffer() noexcept
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
        
        std::ostream &ro_buffer:: display_char(std::ostream &os) const
        {
            const uint8_t *p = static_cast<const uint8_t *>(ro_addr());
            for(size_t i=measure();i>0;--i)
            {
                os << ios::ascii::hybrid[*(p++)];
            }
            return os;
        }


        bool ro_buffer:: has_same_content_than(const ro_buffer &other) const noexcept
        {
            const size_t n = measure();
            if(n==other.measure())
            {
                const uint8_t *p = static_cast<const uint8_t *>(ro_addr());
                const uint8_t *q = static_cast<const uint8_t *>(other.ro_addr());
                for(size_t i=n;i>0;--i)
                {
                    if( *(q++) != *(p++) ) return false;
                }
                return true;
            }
            else
            {
                return false;
            }
        }
    }

}



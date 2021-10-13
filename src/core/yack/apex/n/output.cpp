
#include "yack/apex/natural.hpp"
#include "yack/type/hexa.hpp"
#include <iostream>

namespace yack
{
    namespace apex
    {

        std::ostream & natural:: output_hex(std::ostream &os) const
        {
            if(bytes<=0)
                os << '0';
            else
            {
                // first byte
                {
                    const uint8_t b = (*this)[bytes]; assert(b>0);
                    {
                        const uint8_t up16 = hexa::up16(b);
                        if(up16>0)
                            os << hexa::lowercase_char[up16];
                        os << hexa::lowercase_char[hexa::lo16(b)];
                    }
                }

                // other bytes
                for(size_t i=bytes-1;i>0;--i)
                {
                    os << hexa::lowercase_text[ (*this)[i] ];
                }

            }
            return os;
        }

        std::ostream & operator<<(std::ostream &os, const natural n)
        {
            return n.output_hex(os);
        }


    }

}


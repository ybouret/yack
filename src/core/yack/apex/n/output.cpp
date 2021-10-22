
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
            if( os.flags() & std::ios_base::hex )
            {
                return n.output_hex(os);
            }
            else
            {
                return n.output_dec(os);
            }
        }


        std::ostream & natural:: output_bin(std::ostream &os) const
        {
            size_t ibit = bits();
            if(ibit<=0)
            {
                os << '0';
            }
            else
            {
                while(ibit-- > 0)
                {
                    os << ( bit(ibit) ? '1' : '0' );
                }
            }

            return os;
        }

    }

}
namespace yack
{
    namespace apex
    {
        std::ostream & natural:: output_dec(std::ostream &os) const
        {
            if(words<=0)
            {
                os << '0';
            }
            else
            {
                uint_type    _10 = 10;
                const handle ten(_10);
                natural      tmp(*this);
            }
            return os;
        }

    }

}


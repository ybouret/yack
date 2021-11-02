
#include "yack/ios/fmt/hexa.hpp"
#include "yack/type/hexa.hpp"
#include "yack/arith/base2.hpp"

namespace yack
{

    namespace ios
    {

        size_t hexa:: width(const uint64_t qw) throw()
        {
            return bytes_for(qw);
        }
        
        hexa::hexa(const hexa &_) throw() :
        qword(_.qword),
        count(_.count)
        {
        }

        hexa:: ~hexa() throw() {}


        size_t  hexa:: load(char tab[]) const
        {
            if(count>0)
            {
                const size_t nc = count<<1;
                uint64_t     qw = qword;
                for(size_t i=0;i<nc;++i)
                {
                    tab[i] = yack::hexa::lowercase_char[ uint8_t(qw&0xf) ];
                    qw >>= 4;
                }
                if('0'==tab[nc-1]) return nc-1;
                return nc;
            }
            else
            {
                tab[0] = '0';
                return 1;
            }
        }
    }
}

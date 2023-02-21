
#include "yack/ios/fmt/hexa.hpp"
#include "yack/type/hexa.hpp"
#include "yack/arith/base2.hpp"

namespace yack
{

    namespace ios
    {

        size_t hexa:: width(const uint64_t qw) noexcept
        {
            return bytes_for(qw);
        }
        
        hexa::hexa(const hexa &_) noexcept :
        qword(_.qword),
        count(_.count)
        {
        }

        hexa:: ~hexa() noexcept {}


        size_t  hexa:: load(char tab[]) const noexcept
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

#include <iostream>
#include "yack/ios/ostream.hpp"

namespace yack
{

    namespace ios
    {

        std::ostream & operator<<(std::ostream &os, const hexa &x)
        {
			char tab[16] = { 0 };
            size_t n = x.load(tab); assert(n>0);
            while(n-- > 0) os << tab[n];
            return os;
        }

        ios::ostream & operator<<(ios::ostream &os, const hexa &x)
        {
            char tab[16] = { 0 };
            //size_t n = x.load(tab); assert(n>0);
            os.frame(tab,x.load(tab));
            return os;
        }
    }
}



#include "yack/ios/fmt/binary.hpp"
#include "yack/arith/base2.hpp"

namespace yack
{

    namespace ios
    {

        size_t binary:: width(const uint64_t qw) throw()
        {
            return bits_for(qw);
        }


        binary:: binary(const binary &_) throw() :
        qword(_.qword),
        nbits(_.nbits)
        {
        }

        binary:: ~binary() throw()
        {
        }

        size_t binary:: load(char tab[]) const throw()
        {
            if(nbits>0)
            {
                uint64_t qw = qword;
                for(size_t i=0;i<nbits;++i)
                {
                    tab[i] = '0' + (qw&1);
                    qw >>= 1;
                }
                return nbits;
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
        std::ostream & operator<<(std::ostream &os, const binary &b)
        {
            char tab[64];
            size_t n  = b.load(tab);
            while(n-- > 0) os << tab[n];
            return os;
        }
        
        ios::ostream & operator<<(ios::ostream &os, const binary &b)
        {
            char tab[16];
            os.frame(tab,b.load(tab));
            return os;
        }
        
    }

}



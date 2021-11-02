

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


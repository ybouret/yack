


#include "yack/hashing/ibj64.hpp"

namespace yack
{

    namespace hashing
    {

        uint32_t ibj32(uint32_t a) throw()
        {
            a = (a+0x7ed55d16) + (a<<12);
            a = (a^0xc761c23c) ^ (a>>19);
            a = (a+0x165667b1) + (a<<5);
            a = (a+0xd3a2646c) ^ (a<<9);
            a = (a+0xfd7046c5) + (a<<3);
            a = (a^0xb55a4f09) ^ (a>>16);
            return a;
        }


        ibj64:: ~ibj64() throw() {}

        ibj64::  ibj64() throw() {}


        const char ibj64::clid[] = "IBJ";

        const char *ibj64:: protocol() const throw()  { return clid; }

        void    ibj64::operator()(uint32_t &lword, uint32_t &rword) const throw()
        {
            for(size_t iter=0;iter<4;++iter)
            {
                const uint32_t swp = rword;
                rword = ibj32(rword) ^ lword;
                lword = swp;
            }
        }
    }

}

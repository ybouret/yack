
#include "yack/chem/active.hpp"

namespace yack
{
    namespace chemical
    {

        alist::  alist() noexcept : alist_() {}
        alist:: ~alist() noexcept {}


        alist:: alist(const snode *sn) : alist_()
        {
            alist_ & self = *this;
            for(;sn;sn=sn->next)
            {
                const species &s = ***sn;
                if(s.rank>0)
                    self << s;
            }
        }

    }
    
}

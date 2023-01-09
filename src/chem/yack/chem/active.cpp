
#include "yack/chem/active.hpp"

namespace yack
{
    namespace chemical
    {

        active_list::  active_list() throw() : alist() {}
        active_list:: ~active_list() throw() {}


        active_list:: active_list(const snode *sn) : alist()
        {
            alist & self = *this;
            for(;sn;sn=sn->next)
            {
                const species &s = ***sn;
                if(s.rank)
                    self << s;
            }
        }

    }
    
}

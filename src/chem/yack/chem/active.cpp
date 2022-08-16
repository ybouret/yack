

#include "yack/chem/active.hpp"

namespace yack
{
    namespace chemical
    {
        active_list:: ~active_list() throw()
        {
        }

        active_list:: active_list() throw() : alist() {}

        active_list:: active_list(const snode *node) : alist()
        {
            for(;node;node=node->next)
            {
                const species &sp = ***node;
                if(sp.rank>0) (*this) << &sp;
            }
        }

    }

}

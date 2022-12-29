

#include "yack/chem/active.hpp"

namespace yack
{
    namespace chemical
    {
        active_list:: ~active_list() throw()
        {
        }

        active_list:: active_list() throw() : sp_list() {}

        active_list:: active_list(const snode *node) : sp_list()
        {
            for(;node;node=node->next)
            {
                const species &sp = ***node;
                if(sp.rank>0) (*this) << &sp;
            }
            sort();
        }

    }

}

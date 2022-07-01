
#include "yack/chem/mix/active.hpp"


namespace yack
{
    namespace chemical
    {
        alist:: alist(const library &lib) : alist_()
        {
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &sp = ***node;
                if( sp.rank > 0 ) (*this) << &sp;
            }
        }

        alist:: ~alist() throw()
        {
        }
        
    }

}


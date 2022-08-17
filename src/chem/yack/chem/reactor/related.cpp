
#include "yack/chem/reactor.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        void reactor:: build_related()
        {
            groups &target = coerce(related);
            for(const enode *node=lattice.head();node;node=node->next)
            {
                const equilibrium &rhs = ***node;
                bool               ins = false;
                for(group *g=target.head;g;g=g->next)
                {
                    if(g->accepts(rhs))
                    {
                        (*g) << &rhs;
                        ins = true;
                        break;
                    }
                }
                if(!ins)
                {
                    *(target.push_back( new group() )) << &rhs;
                }
            }

        }
    }
}


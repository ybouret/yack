

#include "yack/chem/eqs/groups.hpp"

namespace yack
{
    namespace chemical
    {

        groups:: ~groups() throw() {}

        groups:: groups() throw() : cxx_list_of<group>()
        {
        }

        std::ostream & operator<<(std::ostream &os, const groups &G)
        {
            os << '{';
            const group *g = G.head;
            if(g)
            {
                os << std::endl << '\t' << *g;
                for(g=g->next;g;g=g->next)
                {
                    os << ',' << std::endl << '\t' << *g;
                }
                os << std::endl;
            }
            os << '}';
            return os;
        }

    }

}

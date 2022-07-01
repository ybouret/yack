#include "yack/chem/mix/groups.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace chemical
    {
        groups:: ~groups() throw() {}

        groups:: groups() throw() : group::list()
        {
        }

        void groups:: sort()
        {
            for(group *g=head;g;g=g->next)
            {
                g->sort();
            }
            merge_list_of<group>::sort(*this,group::compare);
        }

      

        std::ostream & operator<<(std::ostream &os, const groups &G)
        {
            if(G.size>0)
            {
                os << "<groups count=\"" << G.size << "\">" << std::endl;
                for(const group *g=G.head;g;g=g->next)
                {
                    os << "  " << *g << std::endl;
                }
            }
            os << "<groups/>";
            return os;
        }

    }


}

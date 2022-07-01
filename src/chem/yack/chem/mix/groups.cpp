
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

        groups:: groups(const equilibria &eqs) : group::list()
        {
            groups &G = *this;

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                bool               found = false;
                for(group *g= G.head;g;g=g->next)
                {
                    if(g->attached(eq))
                    {
                        (*g) << &eq;
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    *(G.push_back( new group() )) << &eq;
                }
            }

            sort();


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

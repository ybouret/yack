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

#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    namespace chemical
    {
        groups:: groups(const equilibria &lattice, const build_clusters_ &) : group::list()
        {
            // build independent attached groups
            const groups attached(lattice,groups::build_attached);

            // create global detached matrix
            const matrix<bool> detached;
            lattice.build( coerce(detached) );
            //lattice(std::cerr << "detached=",detached,"");

            // create partitions
            const size_t      dims = attached.size;
            cxx_array<groups> part(dims);
            {
                size_t i=0;
                for(const group *g=attached.head;g;g=g->next)
                {
                    groups p(*g,detached);
                    part[++i].swap_with(p);
                }
            }

            // weave
            groups woven(part);
            swap_with(woven);
        }

        groups * groups:: create_from(const equilibria &lattice)
        {
            return new groups(lattice,build_clusters);
        }
    }
}

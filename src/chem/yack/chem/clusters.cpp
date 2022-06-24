#include "yack/chem/clusters.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace chemical
    {
        clusters:: clusters() throw() : cluster::list() {}

        clusters:: ~clusters() throw() {}

        void    clusters:: create_from(const equilibrium &eq)
        {
            cluster *cc = push_back( new cluster() );
            (*cc) << &eq;
        }

        std::ostream & operator<<(std::ostream &os, const clusters &CC)
        {
            os << "<clusters count=\"" << CC.size << "\">" << std::endl;
            for(const cluster *cls=CC.head;cls;cls=cls->next)
            {
                os << "  " << *cls << std::endl;
            }
            os << "<clusters>";
            return os;
        }


        void clusters:: normalize()
        {
            for(cluster *cc=head;cc;cc=cc->next)
            {
                cc->update();
            }
            merge_list_of<cluster>::sort(*this,cluster::compare);
            size_t dim=0;
            for(cluster *cc=head;cc;cc=cc->next)
            {
                ++dim;
                for(vnode *node=cc->head;node;node=node->next)
                {
                    coerce((**node).info) = dim;
                }
            }
        }
    }

}


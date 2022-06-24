
#include "yack/chem/clusters.hpp"

namespace yack
{
    namespace chemical
    {
        clusters:: clusters() throw() : cluster::list() {}

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
    }

}


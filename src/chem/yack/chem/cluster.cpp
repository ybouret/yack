
#include "yack/chem/cluster.hpp"

namespace yack
{
    namespace chemical
    {
        cluster:: ~cluster() throw()
        {

        }

        cluster:: cluster(const size_t i,
                          const equilibrium &eq,
                          const size_t w) :
        object(),
        mlist(),
        indx(i),
        next(0),
        prev(0),
        width(w)
        {
            (*this) << &eq;
        }

        bool cluster:: connected_to(const equilibrium &eq) const throw()
        {
            for(const mnode *node=head;node;node=node->next)
            {
                const equilibrium &mine = **node;
                assert(&mine!=&eq);
                if( equilibrium::connexion(eq,mine) ) return true;
            }
            return false;
        }

        void cluster:: finalize() throw()
        {
            assert(size>0);
            size_t counter=0;
            for(mnode *node=head;node;node=node->next)
            {
                coerce( (**node).isub ) = ++counter;
            }
        }

        std::ostream & operator<<(std::ostream &os, const cluster &cls)
        {
            os << "cluster#" << cls.indx << " [" << cls.size << "] = {" << std::endl;
            for(const mnode *node=cls.head;node;node=node->next)
            {
                const equilibrium &eq = **node;
                eq.display(os << "  [" << eq.isub << "/" << *eq << "] ",cls.width,0); os << std::endl;
            }
            os << "}";
            return os;
        }

    }
}

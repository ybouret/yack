
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
        large_object(),
        mlist(),
        indx(i),
        used(),
        excl(),
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

        bool cluster:: uses(const species &sp) const throw()
        {
            for(const mnode *node=head;node;node=node->next)
            {
                const equilibrium &eq = **node;
                if(eq.uses(sp)) return true;
            }
            return false;
        }

        void cluster:: finalize(const snode *curr) throw()
        {
            assert(size>0);
            {
                size_t counter=0;
                for(mnode *node=head;node;node=node->next)
                {
                    coerce( (**node).isub ) = ++counter;
                }
            }

            while(curr)
            {
                const species &sp = ***curr;
                if(uses(sp))
                {
                    coerce(used) << &sp;
                }
                else
                {
                    coerce(excl) << &sp;
                }
                curr = curr->next;
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
            os << "  [";
            for(const anode *node=cls.used.head;node;node=node->next)
            {
                os << ' ' << '[' << (**node).name << ']';
            }
            os << " ]" << std::endl;
            os << "}";
            return os;
        }

    }
}

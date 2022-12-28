

#include "yack/chem/eqs/repo.hpp"

namespace yack
{
    namespace chemical
    {
        eq_repo:: eq_repo(const eq_fund &fund) throw() : eq_repo_(fund) {}

        eq_repo:: ~eq_repo() throw() {}


        std::ostream & operator<<(std::ostream &os, const eq_repo &self)
        {
            os << '{';
            eq_repo::node_type *node = self.head;
            if(node)
            {
                os << (***node).name;
                for(node=node->next;node;node=node->next)
                {
                    os << ',' << (***node).name;
                }
            }
            os << '}';
            return os;
        }

    }

}



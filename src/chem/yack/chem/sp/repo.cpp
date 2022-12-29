#include "yack/chem/sp/repo.hpp"

namespace yack {

    namespace chemical
    {
        sp_repo:: sp_repo(const sp_fund &fund) throw() : sp_repo_(fund) {}
        sp_repo:: ~sp_repo() throw() {}

        std::ostream & operator<<(std::ostream &os, const sp_repo &self)
        {
            os << '{';
            sp_repo::node_type *node = self.head;
            if(node)
            {
                os << ***node;
                for(node=node->next;node;node=node->next)
                {
                    os << ',' << ***node;
                }
            }
            os << '}';
            return os;
        }


    }

}


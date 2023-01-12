
#include "yack/chem/reactor/eq-repo.hpp"

namespace yack {

    namespace chemical {

        eq_repo:: ~eq_repo() throw() {}

        eq_repo:: eq_repo() throw() : object(), counted(), eq_repo_()  {}


        std::ostream & operator<<(std::ostream &os, const eq_repo &self) {
            os << "<< ";
            eq_node *node = self.head;
            if(node)
            {
                os << (***node).name;
                for(node=node->next;node;node=node->next) {
                    os << ", " << (***node).name;
                }
            }
            os << " >>";
            return os;
        }
    }

}


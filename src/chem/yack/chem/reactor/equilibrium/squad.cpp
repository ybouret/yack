
#include "yack/chem/reactor/equilibrium/squad.hpp"

namespace yack {

    namespace chemical {

        squad:: ~squad() throw()
        {
        }

        squad:: squad(const equilibrium &first) :
        object(), eq_repo_(), next(0), prev(0)
        {
            (*this) << first;
        }

        squad:: squad(const squad &other) : object(), eq_repo_(other), next(0), prev(0) {}

        squad * squad:: clone() const {
            return new squad(*this);
        }

        std::ostream &operator<<(std::ostream &os, const squad &self)
        {
            os << "|> ";
            const eq_node *node = self.head;
            if(node)
            {
                os << (***node).name;
                for(node=node->next;node;node=node->next) {
                    os << ", " << (***node).name;
                }
            }
            os << " <|";
            return os;
        }

    }

}

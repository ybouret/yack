
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

    }

}

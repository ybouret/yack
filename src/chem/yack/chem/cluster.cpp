
#include "yack/chem/cluster.hpp"

namespace yack
{
    namespace chemical
    {
        cluster:: ~cluster() throw()
        {

        }

        cluster:: cluster() throw() : object(), mlist()
        {

        }

        bool cluster:: connected_to(const equilibrium &eq) const throw()
        {
            return false;
        }
    }
}

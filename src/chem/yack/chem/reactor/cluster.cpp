#include "yack/chem/reactor/cluster.hpp"

namespace yack
{
    namespace chemical
    {
        cluster:: ~cluster() throw()
        {
        }
        
        cluster:: cluster() throw() :
        object(),
        next(0),
        prev(0)
        {
            
        }
        
    }
}

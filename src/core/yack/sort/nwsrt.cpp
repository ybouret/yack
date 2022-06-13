#include "nwsrt.hpp"

namespace yack
{
    namespace nwsrt
    {
        swaps:: ~swaps() throw()
        {
        }
        
        swaps:: swaps(const char  *sid,
                      const size_t dim,
                      const size_t num,
                      const size_t *lhs,
                      const size_t *rhs) throw() :
        name(sid),
        size(dim),
        work(num),
        ltab(lhs),
        rtab(rhs)
        {
        }
        
        
    }
}

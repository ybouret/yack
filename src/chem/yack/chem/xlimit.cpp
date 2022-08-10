
#include "yack/chem/xlimit.hpp"

namespace yack
{
    namespace chemical
    {
        xlimit:: ~xlimit() throw()
        {
        }
        
        xlimit:: xlimit(const actor &h,
                        const double x) throw() :
        id(h),
        xi(x)
        {
            assert(xi>=0);
        }
        
        
    }
}

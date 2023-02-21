
#include "yack/chem/xlimit.hpp"

namespace yack
{
    namespace chemical
    {
        xlimit:: ~xlimit() noexcept
        {
        }
        
        xlimit:: xlimit(const actor &h,
                        const double x) noexcept :
        id(h),
        xi(x)
        {
            assert(xi>=0);
        }

        const actor & xlimit:: operator*() const noexcept
        {
            return id;
        }
        
    }
}

#include "nwswp.hpp"

namespace yack
{
    namespace nwsrt
    {
        swaps:: ~swaps() noexcept
        {
        }
        
        swaps:: swaps(const char  *sid,
                      const size_t dim,
                      const size_t num,
                      const size_t *lhs,
                      const size_t *rhs) noexcept :
        name(sid),
        size(dim),
        work(num),
        ltab(lhs),
        rtab(rhs)
        {
        }
        
        
    }
}

#include <iostream>

namespace yack
{
    namespace nwsrt
    {

        static inline void output_swap(const size_t i, const size_t j, const size_t offst)
        {
            std::cerr << "(" << i-offst << "," << j-offst << ")";
        }

        void swaps:: display(const size_t offset) const
        {
            std::cerr << name << "=[";
            output_swap(ltab[0],rtab[0],offset);
            for(size_t i=1;i<size;++i)
            {
                std::cerr << ';'; output_swap(ltab[i],rtab[i],offset);
            }
            std::cerr << "]" << std::endl;
        }

    }

}

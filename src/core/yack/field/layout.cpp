
#include "yack/field/layout.hpp"

namespace yack
{
    namespace kernel
    {
        layout:: ~layout() throw()
        {
            coerce(dims)=0;
            coerce(items)=0;
        }

        layout:: layout(const size_t d) throw() :
        dims(d),
        items(0)
        {
        }

        layout:: layout(const layout &other) throw() :
        dims(other.dims),
        items(other.items)
        {
        }
        

        void layout:: setup(unit_t *lo, unit_t *hi, unit_t *w) throw()
        {
            assert(lo);
            assert(hi);
            assert(w);
            size_t &prod = coerce(items);
            prod = 1;
            for(size_t i=0;i<dims;++i)
            {
                unit_t &l = lo[i];
                unit_t &h = hi[i];
                if(l>h) cswap(l,h);
                prod *= ( w[i] = 1+h-l );
            }
        }
    }
}

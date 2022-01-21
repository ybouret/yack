
#include "yack/field/layout.hpp"

namespace yack
{
    namespace kernel
    {
        layout:: ~layout() throw()
        {
            coerce(space)=0;
            coerce(items)=0;
        }

        layout:: layout(const size_t d) throw() :
        space(d),
        items(0)
        {
        }

        layout:: layout(const layout &other) throw() :
        space(other.space),
        items(other.items)
        {
        }
        

        void layout:: finalize_(unit_t *lower,
                                unit_t *upper,
                                unit_t *width,
                                unit_t *pitch)   throw()
        {
            assert(lower);
            assert(upper);
            assert(width);
            size_t &prod = coerce(items);
            prod = 1;
            for(size_t i=0;i<space;++i)
            {
                unit_t &l = lower[i];
                unit_t &h = upper[i];
                if(l>h) cswap(l,h);
                prod *= ( width[i] = h-l+1 );
            }
            pitch[0]=1;
            for(size_t i=1;i<space;++i)
            {
                pitch[i]=pitch[i-1]*width[i-1];
            }
        }

        bool layout:: contains_(const unit_t *cr,const unit_t *lo,const unit_t *hi) const throw()
        {
            assert(cr!=NULL);
            assert(lo!=NULL);
            assert(hi!=NULL);

            for(size_t i=space;i>0;--i)
            {
                const unit_t here = *(cr++);
                if(here<*(lo++)) return false;
                if(here>*(hi++)) return false;
            }

            return true;

        }

        unit_t layout:: index_of_(const unit_t *cr, const unit_t *lower, const unit_t *pitch) const throw()
        {
            unit_t res = cr[0] - lower[0];
            for(size_t i=1;i<space;++i)
            {
                res += (cr[i]-lower[i]) * pitch[i];
            }
            return res;
        }

    }
}

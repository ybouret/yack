
#include "yack/aqueous/eqs/boundaries.hpp"

namespace yack
{
    namespace aqueous
    {
        
        boundaries:: ~boundaries() noexcept
        {

        }

        boundaries:: boundaries(const zl_proxy &zlp,
                                const sp_proxy &spp) noexcept :
        zlimits(zlp),
        spore(spp)
        {
        }

        void boundaries:: insert1(const double xi, const species &sp)
        {
            assert(1==size);
            zlimit &zl = **head;
            switch( __sign::of(xi,zl.extent) )
            {
                case negative: (void) shove(spore,xi,sp); return; // lt
                case positive: (void) annex(spore,xi,sp); return; // gt
                case __zero__: zl << sp;                  return; // eq
            }
        }

        void  boundaries:: insert(const double   xi,
                                  const species &sp)
        {
            assert(xi<0);
            switch(size)
            {
                case 0: (void) annex(spore,xi,sp); return; //!< initialize
                case 1: insert1(xi,sp);            return; //!< special case
                default:
                    break;
            }

            assert(size>=2);
            

        }

    }
}

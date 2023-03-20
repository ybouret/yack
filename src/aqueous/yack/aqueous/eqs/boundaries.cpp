
#include "yack/aqueous/eqs/boundaries.hpp"
#include "yack/exception.hpp"

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
                case 0: (void) annex(spore,xi,sp); return; //!< initialize with new node
                case 1: insert1(xi,sp);            return; //!< special case
                default:
                    break;
            }

            assert(size>=2);

            zl_node *lower = head;
            {
                zlimit &zhead = **lower;
                switch( __sign::of(xi,zhead.extent) )
                {
                    case negative: (void) shove(spore,xi,sp); return; // lt lower
                    case __zero__: zhead << sp;               return; // eq lower
                    case positive: break;                             // gt lower => break
                }
            }

            zl_node *upper = tail; assert(upper!=lower);
            {
                zlimit &ztail = **upper;
                switch( __sign::of(xi,ztail.extent) )
                {
                    case positive: (void) annex(spore,xi,sp); return; // gt upper
                    case __zero__: ztail << sp;               return; // eq upper
                    case negative: break;                             // lt upper => break
                }
            }

            throw exception("not implemented");


        }

    }
}

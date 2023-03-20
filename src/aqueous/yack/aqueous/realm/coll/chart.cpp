
#include "yack/aqueous/realm/coll/chart.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace collecting
        {

            chart:: ~chart() noexcept
            {
            }

            chart:: chart(const caches &cc) noexcept :
            reac(cc.zlp,cc.spp),
            prod(cc.zlp,cc.spp)
            {
            }

            bool chart:: settle(const components       &E,
                                const readable<double> &C,
                                const readable<bool>   &R)
            {
                reac.detect(E.reac,C,R);
                prod.detect(E.prod,C,R);
                return (reac.oor.size<=0 && prod.oor.size<=0);
            }

            std::ostream & operator<<(std::ostream &os, const chart &self)
            {
                os <<   self.reac << " <=> " << self.prod;
                return os;
            }

        }
    }
}


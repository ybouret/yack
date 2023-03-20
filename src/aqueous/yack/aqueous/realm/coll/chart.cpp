
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

            void chart:: operator()(const components       &E,
                                    const readable<double> &C)
            {
                reac.detect(E.reac,C);
                prod.detect(E.prod,C);
            }

            std::ostream & operator<<(std::ostream &os, const chart &self)
            {
                os << "reac: " << self.reac << " | prod: " << self.prod;
                return os;
            }

        }
    }
}


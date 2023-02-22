#include "yack/cameo/add.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;

namespace {

    template <typename T>
    static inline
    void perform_add( randomized::bits &ran )
    {
        cameo::add<T> xadd;

        for(size_t n=0;n<=10;++n)
        {
            std::cerr << "\tn=" << n << std::endl;
            xadd.free();
            T    sum = 0;
            for(size_t i=0;i<n;++i) {
                const T tmp = ran.choice() ? -bring::get<T>(ran) : bring::get<T>(ran);
                xadd.push(tmp);
                sum += tmp;
                //std::cerr << xadd << std::endl;
            }
            std::cerr << "\t\t" << sum << " / " << xadd.sum() << std::endl;
        }
    }

}

YACK_UTEST(cameo_add)
{
    randomized::rand_ ran;

    perform_add<int>(ran);
    perform_add<float>(ran);
    perform_add<double>(ran);
    perform_add<long double>(ran);


}
YACK_UDONE()

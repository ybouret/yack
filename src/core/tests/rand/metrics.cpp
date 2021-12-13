

#include "yack/utest/run.hpp"
#include "yack/randomized/metrics.hpp"
#include "yack/type/ints.hpp"
#include "yack/ios/fmt/hexa.hpp"


#include <typeinfo>
#include <cstring>

using namespace yack;


template <typename T>
static inline void show_metrics()
{
    std::cerr << "Using <" << typeid(T).name() << ">" << std::endl;
    YACK_OUTPUT(randomized::metrics::from<T>::bits);
    YACK_OUTPUT(randomized::metrics::from<T>::maxi);
    std::cerr << "\t-> " << ios::hexa(randomized::metrics::from<T>::maxi) << std::endl;

    {
        randomized::metrics::from<T> m(1000000);
        std::cerr << "compacted: " << m.compacted << std::endl;
        std::cerr << "unit_deno: " << m.unit_deno << std::endl;
        uint32_t u = 10;
        std::cerr << m.unit(u) << std::endl;
    }

    {
        randomized::metrics::from<T> m(0xffffffff);
        std::cerr << "compacted: " << m.compacted << std::endl;
        std::cerr << "unit_deno: " << m.unit_deno << std::endl;
        uint32_t u = 10;
        std::cerr << m.unit(u) << std::endl;
    }


}

YACK_UTEST(rand_metrics)
{
    show_metrics<float>();
    //show_metrics<double>();
    //show_metrics<long double>();



}
YACK_UDONE()


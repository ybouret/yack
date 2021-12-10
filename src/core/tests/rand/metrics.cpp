

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
    YACK_OUTPUT(randomized::metrics_of<T>::system_bits);
    YACK_OUTPUT(randomized::metrics_of<T>::system_mask);
    std::cerr << "\t-> " << ios::hexa(randomized::metrics_of<T>::system_mask) << std::endl;

    randomized::metrics_of<T> m(65535);
    
    

}

YACK_UTEST(rand_metrics)
{
    show_metrics<float>();
    show_metrics<double>();
    show_metrics<long double>();



}
YACK_UDONE()


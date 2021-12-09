

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
    YACK_OUTPUT(randomized::metrics_of<T>::max_bits);
    YACK_OUTPUT(randomized::metrics_of<T>::max_mask);
    std::cerr << "\t-> " << ios::hexa(randomized::metrics_of<T>::max_mask) << std::endl;

    const T rmin = randomized::metrics_of<T>::word_to_unit(0);
    std::cerr << "\t-> rmin = " << rmin << std::endl;

    const T rmax = randomized::metrics_of<T>::word_to_unit(randomized::metrics_of<T>::max_mask);
    std::cerr << "\t-> 1-rmax = " << (1-rmax) << std::endl;
    YACK_CHECK(T(1.0)-rmax>0);

}

YACK_UTEST(rand_metrics)
{
    show_metrics<float>();
    show_metrics<double>();
    show_metrics<long double>();

}
YACK_UDONE()


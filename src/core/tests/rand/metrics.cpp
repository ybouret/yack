

#include "yack/utest/run.hpp"
#include "yack/randomized/metrics.hpp"
#include "yack/type/ints.hpp"
#include "yack/ios/fmt/hexa.hpp"


#include <typeinfo>
#include <cstring>
#include <cstdio>

using namespace yack;


template <typename T> static inline
void show(randomized::u32_to<T> &m, const uint32_t u)
{
    const T value = m.unit(u);
    std::cerr << "\tunit(" << std::setw(10) << u << ") = " << value << std::endl;
}

template <typename T>
static inline void show_metrics()
{
    std::cerr << "Using <" << typeid(T).name() << ">" << std::endl;
    YACK_OUTPUT(randomized::u32_to<T>::bits);
    YACK_OUTPUT(randomized::u32_to<T>::maxi);
    std::cerr << "\t-> " << ios::hexa(randomized::u32_to<T>::maxi) << std::endl;

    {
        const uint32_t small=1000000;
        randomized::u32_to<T> m(small);
        std::cerr << "compacted: " << m.compacted << std::endl;
        std::cerr << "unit_deno: " << m.unit_deno << std::endl;
        std::cerr << "symm_deno: " << m.symm_deno << std::endl;
        std::cerr << "unit:" << std::endl;
        show(m,0);
        show(m,small/2);
        show(m,small);
        YACK_CHECK(T(1)-m.unit(small)>0);

    }

    {
        const uint32_t big=0xffffffff;
        randomized::u32_to<T> m(big);
        std::cerr << "compacted: " << m.compacted << std::endl;
        std::cerr << "unit_deno: " << m.unit_deno << std::endl;
        std::cerr << "symm_deno: " << m.symm_deno << std::endl;
        std::cerr << "unit:" << std::endl;
        show(m,0);
        show(m,big/2);
        show(m,big);
        YACK_CHECK(T(1)-m.unit(big)>0);
    }

    std::cerr << std::endl;

}

YACK_UTEST(rand_metrics)
{
    show_metrics<long double>();
    show_metrics<double>();
    show_metrics<float>();

}
YACK_UDONE()


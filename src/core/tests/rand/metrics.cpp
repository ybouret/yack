

#include "yack/utest/run.hpp"
#include "yack/randomized/metrics.hpp"
#include "yack/type/ints.hpp"
#include "yack/ios/fmt/hexa.hpp"


#include <typeinfo>
#include <cstring>
#include <cstdio>

using namespace yack;


template <typename T> static inline
void showU(randomized::u32_to<T> &m, const uint32_t u)
{
    const T value = m.unit(u);
    std::cerr << "\tunit(" << std::setw(10) << u << ") = " << value << std::endl;
}

template <typename T> static inline
void showS(randomized::u32_to<T> &m, const uint32_t u)
{
    const T value = m.symm(u);
    std::cerr << "\tsymm(" << std::setw(10) << u << ") = " << value << std::endl;
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
        std::cerr << "unit:" << std::endl;
        showU(m,0);
        showU(m,small/2);
        showU(m,small);

        showS(m,0);
        showS(m,small/2);
        showS(m,small);

        YACK_CHECK(m.unit(0)>0);
        YACK_CHECK(T(1)-m.unit(small)>0);
        YACK_CHECK(T(1)+m.symm(0)>0);
        YACK_CHECK(T(1)-m.symm(small)>0);

    }

    {
        const uint32_t big=0xffffffff;
        randomized::u32_to<T> m(big);
        std::cerr << "compacted: " << m.compacted << std::endl;
        std::cerr << "unit_deno: " << m.unit_deno << std::endl;
        std::cerr << "unit:" << std::endl;
        showU(m,0);
        showU(m,big/2);
        showU(m,big);
        showS(m,0);
        showS(m,big/2);
        showS(m,big);

        YACK_CHECK(m.unit(0)>0);
        YACK_CHECK(T(1)-m.unit(big)>0);
        YACK_CHECK(T(1)+m.symm(0)>0);
        YACK_CHECK(T(1)-m.symm(big)>0);
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


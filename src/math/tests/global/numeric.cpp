
#include "yack/math/numeric.hpp"
#include "yack/utest/run.hpp"
#include <typeinfo>

using namespace yack;
using namespace math;


#define NUM_DISP(VALUE) std::cerr << std::setw(32) << #VALUE << "<" << id << "> = " << numeric<T>::VALUE << std::endl

namespace
{
    template <typename T>
    static inline void display()
    {
        const char *id = typeid(T).name();
        NUM_DISP(eps);
        NUM_DISP(minimum);
        NUM_DISP(maximum);

        std::cerr << std::endl;
    }
}


YACK_UTEST(numeric)
{
    display<float>();
    display<double>();
    display<long double>();

}
YACK_UDONE()


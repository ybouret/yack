
#include "yack/math/numeric.hpp"
#include "yack/math/timings.hpp"

#include "yack/utest/run.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace math;


#define NUM_DISP(VALUE) std::cerr << std::setw(32) << #VALUE << "<" << id << "> = " << numeric<T>::VALUE << std::endl

namespace
{
    template <typename T>
    static inline void display()
    {
        const string &id = rtti::name<T>();
        
        NUM_DISP(epsilon);
        NUM_DISP(minimum);
        NUM_DISP(maximum);
        NUM_DISP(golden_r);
        NUM_DISP(golden_c);
        NUM_DISP(golden_i);
        NUM_DISP(dig);
        NUM_DISP(ftol);

        const T a  = T(1.1e-4);  std::cerr << " a = " << a << std::endl;
        const T fa = timings::round_floor(a); std::cerr << "fa = " << fa << std::endl;
        const T ca = timings::round_ceil(a);  std::cerr << "ca = " << ca << std::endl;

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


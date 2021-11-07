#include "yack/math/algebra/lu.hpp"
#include "yack/type/complex.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include "../../../core/tests/main.hpp"

using namespace yack;
using namespace math;

template <typename T>
static inline void do_LU(const size_t n, randomized::bits &ran)
{
    std::cerr << "LU<" << typeid(T).name() << ">(" << n << ")" << std::endl;
    lu<T> LU(n);
    std::cerr << "|_granted: " << LU.granted() << std::endl;
    
    matrix<T> a(n,n);
    
    bring::fill(a,ran);
    std::cerr << "a=" << a << std::endl;
    
    if(LU.build(a))
    {
        std::cerr << "ok" << std::endl;
    }
    else
    {
        std::cerr << "singular" << std::endl;
    }
    
    
}

YACK_UTEST(lu)
{
    randomized::rand_ ran;
    
    for(size_t dim=1;dim<=1;++dim)
    {
        do_LU<float>(dim,ran);
        do_LU<double>(dim,ran);
        do_LU<long double>(dim,ran);
        do_LU< complex<float>       >(dim,ran);
        do_LU< complex<double>      >(dim,ran);
        do_LU< complex<long double> >(dim,ran);
        do_LU<apq>(dim,ran);
    }
}
YACK_UDONE()


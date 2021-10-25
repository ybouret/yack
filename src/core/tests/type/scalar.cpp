#include "yack/type/complex.hpp"
#include "yack/utest/run.hpp"
#include <typeinfo>

using namespace yack;

namespace
{

    template <typename T>
    static inline
    void test_scalar_for()
    {
        std::cerr << "scalar_for<" << typeid(T).name() << ">: ";
        typedef typename scalar_for<T>::type stype;
        std::cerr << typeid(stype).name();
        std::cerr << std::endl;
    }

}

YACK_UTEST(type_scalar)
{
    test_scalar_for<int>();
    test_scalar_for<float>();
    test_scalar_for<double>();
    test_scalar_for< complex<float> >();
    test_scalar_for< complex<double> >();
    test_scalar_for< complex<long double> >();

}
YACK_UDONE()


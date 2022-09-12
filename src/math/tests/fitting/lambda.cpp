#include "yack/math/fitting/lambda.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/rtti.hpp"

using namespace yack;
using namespace math;

namespace
{
    template <typename T>
    static inline
    void show_lam()
    {
        static const string &id = rtti::name<T>();

        std::cerr << "lambda<" << id << ">::pmin = " << fitting::lambda<T>::pmin() << std::endl;
        std::cerr << "lambda<" << id << ">::pmax = " << fitting::lambda<T>::pmax() << std::endl;

        const fitting::lambda<T> lam;
        std::cerr << lam << std::endl;
        lam.print(std::cerr) << std::endl;
    }
}

YACK_UTEST(fitting_lambda)
{
    show_lam<long double>();
    show_lam<double>();
    show_lam<float>();

}
YACK_UDONE()



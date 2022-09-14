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
        //lam.print(std::cerr) << std::endl;
        int p = 0;
        T   f = lam.initialized(p);
        std::cerr << "init: " << std::setw(15) << f << " @" << p << std::endl;
        do
        {
            lam.decrease(p);
            std::cerr << "      " << std::setw(15) << lam[p] << " @" << p << std::endl;
        } while(lam[p]>0);

        while( lam.increase(p) )
        {
            YACK_ASSERT(lam[p]>0);
        }
        std::cerr << "      " << std::setw(15) << lam[p] << " @" << p << std::endl;
        std::cerr << "ptol= " << std::setw(15) << lam.ptol() << std::endl;

        
    }
}

#include <cfloat>

YACK_UTEST(fitting_lambda)
{
    show_lam<long double>();
    show_lam<double>();
    show_lam<float>();



}
YACK_UDONE()



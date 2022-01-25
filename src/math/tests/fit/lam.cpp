#include "yack/math/fit/least-squares/lambda.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

namespace
{
    template <typename T>
    static inline
    void show_lam(  )
    {
        const fit::lambda<T> lam;
        std::cerr << "layout=" << lam << " / pmin=" << lam.pmin() << " / pmax=" << lam.pmax() << std::endl;
        YACK_CHECK(std::abs(lam[lam.pmin()])<=0);
        for(unit_t i=lam.pmin()+1;i<=lam.pmax();++i)
        {
            const T p10 =  floor(std::log10(lam[i]) + T(0.5));
            const unit_t power_of_ten(p10);
            //std::cerr << "lam[" << i << "]=" << lam[i] << " => " << p10 << std::endl;
            YACK_ASSERT(power_of_ten==i);
        }
    }
}

YACK_UTEST(fit_lam)
{
    show_lam<float>();
    show_lam<double>();
    show_lam<long double>();

}
YACK_UDONE()





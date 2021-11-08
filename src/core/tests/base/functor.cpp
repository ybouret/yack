#include "yack/functor.hpp"
#include "yack/utest/run.hpp"

#include <cmath>

using namespace yack;

namespace
{

    class dummy
    {
    public:
        float slope;
        dummy(const float a)      throw() : slope(a)           {}
        dummy(const dummy &other) throw() : slope(other.slope) {}

        double operator()(double x)
        {
            return slope*x;
        }

        double compute(double x)
        {
            return slope*x;
        }


    private:
        YACK_DISABLE_ASSIGN(dummy);
    };

    double Sin(double x)
    {
        return sin(x);
    }
}

YACK_UTEST(functor)
{
    typedef functor<double,TL1(double)> func_type;

    func_type f0  = cfunctor(Sin);
    dummy     d1(2);
    func_type f1 = d1;
    std::cerr << f1(1) << std::endl;
    d1.slope = 4;
    std::cerr << f1(1) << std::endl;
    func_type f2(&d1, & dummy::compute );
    std::cerr << f2(1) << std::endl;
    d1.slope = 8;
    std::cerr << f2(1) << std::endl;
    std::cerr << f1(1) << std::endl;
    std::cerr << f0(1) << std::endl;

}
YACK_UDONE()



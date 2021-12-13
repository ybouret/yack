#include "yack/randomized/metrics.hpp"


namespace yack
{
    namespace randomized
    {


        metrics:: metrics(const uint32_t umax) throw() :
        m(umax),
        f(umax),
        d(umax),
        l(umax)
        {
        }


        metrics:: ~metrics() throw()
        {

        }

        template <>
        float metrics:: unit<float>(const uint32_t u) const throw()
        {
            return f.unit(u);
        }

        template <>
        double metrics:: unit<double>(const uint32_t u) const throw()
        {
            return d.unit(u);
        }

        template <>
        long double metrics:: unit<long double>(const uint32_t u) const throw()
        {
            return l.unit(u);
        }

    }

}



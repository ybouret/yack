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
        float metrics:: unit_<float>(const uint32_t u) const throw()
        {
            return f.unit(u);
        }

        template <>
        double metrics:: unit_<double>(const uint32_t u) const throw()
        {
            return d.unit(u);
        }

        template <>
        long double metrics:: unit_<long double>(const uint32_t u) const throw()
        {
            return l.unit(u);
        }

        template <>
        float metrics:: symm_<float>(const uint32_t u) const throw()
        {
            return f.symm(u);
        }

        template <>
        double metrics:: symm_<double>(const uint32_t u) const throw()
        {
            return d.symm(u);
        }

        template <>
        long double metrics:: symm_<long double>(const uint32_t u) const throw()
        {
            return l.symm(u);
        }





    }

}



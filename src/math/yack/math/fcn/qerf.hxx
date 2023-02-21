
namespace yack
{
    namespace math
    {

        static inline
        real_t qerf_core(const real_t &x) noexcept
        {

            static const real_t one(1);
            static const real_t half(0.5);

            const real_t z = std::abs(x);
            const real_t t = one/(one+half*z);
            return t*exp(-z*z-1.26551223+t*(1.00002368+t*(0.37409196+t*(0.09678418+ t*(-0.18628806+t*(0.27886807+t*(-1.13520398+t*(1.48851587+t*(-0.82215223+t*0.17087277)))))))));
        }

        template <>
        real_t qerfc<real_t>(const real_t x) noexcept
        {
            static const real_t two(2);
            const real_t        ans = qerf_core(x);
            return x >= 0 ? ans : two-ans;
        }

        template <>
        real_t qerf<real_t>(const real_t x) noexcept
        {
            static const real_t two(2);
            const real_t        ans = qerf_core(x);
            return x >= 0 ? two - ans : ans;
        }

    }

}

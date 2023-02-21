

namespace yack
{
    namespace math
    {

        template <>
        polynomial:: interpolate<real_t>:: interpolate(const size_t ndat) :
        tableaux(2,ndat),
        c(next()),
        d(next())
        {
        }

        template <>
        polynomial:: interpolate<real_t>:: ~interpolate() noexcept
        {
        }

        template <>
        void polynomial:: interpolate<real_t>:: throw_singular() const
        {
            throw libc::exception(EDOM,"singular abscissae in polynomial interpolation");
        }


        
    }
}

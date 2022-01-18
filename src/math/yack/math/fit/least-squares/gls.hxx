
namespace yack
{
    namespace math
    {
        namespace fit
        {
            template <>
            gls<real_t>:: gls() : least_squares<real_t,real_t>()
            {
            }

            template <>
            gls<real_t>:: ~gls() throw()
            {
            }
        }
    }
}


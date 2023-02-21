
namespace yack
{
    namespace math
    {
        template <>
        gradient<real_t>:: gradient(const derivative_ptr &p,
                                    const real_t          h) noexcept :
        drvs(p),
        scal(h)
        {
        }
        
        template <>
        gradient<real_t>:: ~gradient() noexcept
        {
            
        }
    }
}

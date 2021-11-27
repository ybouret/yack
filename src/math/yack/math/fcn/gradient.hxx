
namespace yack
{
    namespace math
    {
        template <>
        gradient<real_t>:: gradient(const derivative_ptr &p,
                                    const real_t          h) throw() :
        drvs(p),
        scal(h)
        {
        }
        
        template <>
        gradient<real_t>:: ~gradient() throw()
        {
            
        }
    }
}



namespace yack
{
    namespace math
    {
        template <>
        jacobian<real_t>:: jacobian(const derivative_ptr &p,
                                    const real_t          h) throw() :
        drvs(p),
        scal(h)
        {
        }
        
        template <>
        jacobian<real_t>:: ~jacobian() throw()
        {
            
        }
    }
}

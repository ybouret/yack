

namespace yack
{
    namespace math
    {
        template <>
        jacobian<real_t>:: jacobian(const size_t ndat, const real_t h) throw() :
        tableaux(2,ndat),
        scal(h),
        fp( next() ),
        fm( next() )
        {
        }
        
        template <>
        jacobian<real_t>:: ~jacobian() throw()
        {
            
        }

        template <>
        real_t jacobian<real_t>:: regp(const real_t xx, real_t &hh)
        {
            volatile real_t ans = xx + hh;
            hh = ans - xx;
            if( fabs(hh)<=0 ) throw libc::exception(ERANGE,"jacobian positive underflow");
            return ans;
        }

        template <>
        real_t jacobian<real_t>:: regm(const real_t xx, real_t &hh)
        {
            volatile real_t ans = xx - hh;
            hh = xx-ans;
            if( fabs(hh)<=0 ) throw libc::exception(ERANGE,"jacobian negative underflow");
            return ans;
        }

    }
}

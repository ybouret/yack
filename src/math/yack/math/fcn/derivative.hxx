
namespace yack
{

    namespace math
    {

        template <>
        derivative<real_t>:: ~derivative() noexcept {}


#define YACK_DRVS_CTRL 1.4
#define YACK_DRVS_NTAB 10

        template <>
        derivative<real_t>:: derivative() :
        a(YACK_DRVS_NTAB,YACK_DRVS_NTAB)
        {
        }

        template <>
        real_t derivative<real_t>:: unit_step_size() noexcept
        {
            static const real_t one(1);
            static const real_t three(3);
            static const real_t third = one/three;
            static const real_t value = timings::round_floor<real_t>( pow(numeric<real_t>::epsilon,third) );
            return value;
        }

        template <>
        real_t derivative<real_t>:: diff_maxi_ftol() noexcept
        {
            static const real_t value = timings::round_ceil<real_t>( sqrt( numeric<real_t>::epsilon ) );
            return value;
        }

        template <>
        void derivative<real_t>:: regularize(const real_t x, real_t &h)
        {
            volatile real_t temp = x+h;
            if(std::abs( (h = temp-x) ) <= 0)
                throw libc::exception(ERANGE,"underflow in derivative");

        }


        template <>
        void derivative<real_t>:: rescale(const real_t x, real_t &h)
        {
            static const real_t den(YACK_DRVS_CTRL);
            regularize(x,h/=den);
        }

        template <>
        bool derivative<real_t>:: converged(real_t &ans, const size_t i, real_t &err) noexcept
        {
            assert(i>=2);
            static const real_t one(1);
            static const real_t mul(YACK_DRVS_CTRL*YACK_DRVS_CTRL);
            real_t              fac = mul;
            bool                ini = (2==i);
            const size_t        im  = i-1;
            for(size_t j=2,jm=1;j<=i;++j,++jm)
            {
                a[j][i]=(a[jm][i]*fac-a[jm][im])/(fac-one);

                const real_t errt=max_of(std::abs(a[j][i]-a[jm][i]),std::abs(a[j][i]-a[jm][im]));
                if(ini || (errt<=err) )
                {
                    err = errt;
                    ans = a[j][i];
                    ini = false;
                }
                fac=mul*fac;
            }
            return ( std::abs(a[i][i]-a[im][im]) >= twice(err) );
        }


        template <>
        real_t derivative<real_t>:: diff_( real_function<real_t> &f, const real_t x, real_t h, real_t &err)
        {
            const size_t dim = a.rows; regularize(x,h);
            real_t       ans = (a[1][1] = ( f(x+h)-f(x-h) ) / (h+h));
            for(size_t i=2;i<=dim;++i)
            {
                rescale(x,h);
                a[1][i] = ( f(x+h)-f(x-h) ) / (h+h);
                if(converged(ans,i,err)) return ans;
            }
            return ans;
        }


        template <>
        real_t derivative<real_t>:: diff_( real_function<real_t> &f, const real_t x, real_t h)
        {
            static const real_t max_ftol = diff_maxi_ftol();
            real_t err  = 0;
            real_t dFdx = diff_(f,x,h,err);
            while(err>max_ftol*std::abs(dFdx) )
            {
                real_t       new_err  = 0;
                const real_t new_dFdx = diff_(f,x,h/=2,new_err);
                if(new_err>=err)
                {
                    break; // not better
                }
                err  = new_err;
                dFdx = new_dFdx;
            }
            return dFdx;
        }

    }


}

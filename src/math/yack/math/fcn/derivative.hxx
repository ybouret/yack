
namespace yack
{

    namespace math
    {

        template <>
        derivative<real_t>:: ~derivative() throw() {}


#define YACK_DRVS_CTRL 1.4
#define YACK_DRVS_NTAB 10

        template <>
        derivative<real_t>:: derivative() :
        a(YACK_DRVS_NTAB,YACK_DRVS_NTAB)
        {
        }

        template <>
        void derivative<real_t>:: regularize(const real_t x, real_t &h)
        {
            volatile real_t temp = x+h;
            if(fabs( (h = temp-x) ) <= 0)
                throw libc::exception(ERANGE,"underflow in derivative");

        }


        template <>
        void derivative<real_t>:: rescale(const real_t x, real_t &h)
        {
            static const real_t den(YACK_DRVS_CTRL);
            regularize(x,h/=den);
        }

        template <>
        bool derivative<real_t>:: converged(real_t &ans, const size_t i, real_t &err) throw()
        {
            assert(i>=2);
            static const real_t one(1);
            static const real_t mul(YACK_DRVS_CTRL*YACK_DRVS_CTRL);
            real_t              fac = mul;
            for(size_t j=2;j<=i;++j)
            {
                a[j][i]=(a[j-1][i]*fac-a[j-1][i-1])/(fac-one);

                const real_t errt=max_of(fabs(a[j][i]-a[j-1][i]),fabs(a[j][i]-a[j-1][i-1]));
                if(i==2 || (errt<=err) )
                {
                    err = errt;
                    ans = a[j][i];
                }
                fac=mul*fac;
            }
            return ( fabs(a[i][i]-a[i-1][i-1]) >= twice(err) );
        }


    }


}

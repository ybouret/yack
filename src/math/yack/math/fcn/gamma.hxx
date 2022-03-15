

namespace yack
{
    namespace math
    {

        template <>
        real_t gammaln<real_t>(const real_t xx)
        {
            if(xx<=0) throw libc::exception(EDOM,"gammaln(%g)", double(xx));
            static const double cof[6]=
            {
                76.18009172947146,    -86.50532032941677,
                24.01409824083091,    -1.231739572450155,
                0.1208650973866179e-2,-0.5395239384953e-5 };
            double y   = xx;
            double x   = y;
            double tmp = x+5.5;tmp -= (x+0.5)*log(tmp);
            double ser = 1.000000000190015;
            for(unsigned j=0;j<=5;j++)
            {
                ser += cof[j]/++y;
            }
            return static_cast<real_t>(-tmp+log(2.5066282746310005*ser/x));
        }

        template <>
        real_t beta<real_t>(const real_t w, const real_t z)
        {
            return exp( gammaln(w)+gammaln(z) - gammaln(w+z) );
        }

        template <> real_t factln<real_t>(const int n)
        {
            if(n<0)
            {
                throw libc::exception(EDOM,"factln(%d)",n);
            }
            else
            {
                return 0;
            }
        }

    }

}

#include "yack/apex/natural.hpp"

//#include <iomanip>

namespace yack
{
    namespace apex
    {

        double natural:: to_double() const
        {
            static const double fac = double(word_base);
            double ans = 0;
            for(size_t i=words;i>0;)
            {
                const word_type w = word[--i];
                ans *= fac;
                ans += w;
            }
            return ans;
        }

        double natural:: ratio(const natural &num, const natural &den)
        {
            static const double fac[16] =
            {
                1e-1, 1e-2,  1e-3,  1e-4,  1e-5,  1e-6,  1e-7,  1e-8,
                1e-9, 1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15, 1e-16
            };

            uint_type    _10 = 10;
            const handle ten(_10);
            natural r;
            natural q = quot(num,den,r);
            double  ans = q.to_double();
            for(size_t i=0;i<sizeof(fac)/sizeof(fac[0]);++i)
            {
                const handle  rh(r);
                const natural nn = mul(rh,ten);
                q = quot(nn,den,r);
                ans += fac[i] * double( q.lsu() );
            }



            return ans;
        }

    }

}



#include "yack/apex/natural.hpp"


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
            double  ans = 0;
            natural r;
            natural q = quot(num,den,r);
            
            return ans;
        }

    }

}



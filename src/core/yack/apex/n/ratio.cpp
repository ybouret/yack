#include "yack/apex/natural.hpp"


namespace yack
{
    namespace apex
    {


        
        namespace
        {
            template <typename T> struct ap_ratio
            {
                static const T      fac[];
                static const size_t num;
            };

            template <> const float ap_ratio<float>::fac[] =
            {
                1e-1f, 1e-2f,  1e-3f,  1e-4f,  1e-5f,  1e-6f,  1e-7f,  1e-8f
            };

            template <> const size_t ap_ratio<float>::num = sizeof(fac)/sizeof(fac[0]);

            template <> const double ap_ratio<double>:: fac[] =
            {
                1e-1, 1e-2,  1e-3,  1e-4,  1e-5,  1e-6,  1e-7,  1e-8,
                1e-9, 1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15, 1e-16, 1e-17
            };

            template <> const size_t ap_ratio<double>::num = sizeof(fac)/sizeof(fac[0]);


            template <> const long double ap_ratio<long double>:: fac[] =
            {
                1e-1, 1e-2,  1e-3,  1e-4,  1e-5,  1e-6,  1e-7,  1e-8,
                1e-9, 1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15, 1e-16, 1e-17, 1e-18, 1e-19
            };

            template <> const size_t ap_ratio<long double>::num = sizeof(fac)/sizeof(fac[0]);

        }


        template <>
        float natural:: ratio<float>(const natural &num, const natural &den)
        {

            uint_type    _10 = 10;
            const handle ten(_10);
            natural r;
            natural q  = quot(num,den,r);
            float  ans = q.to<float>();
            for(size_t i=0;i<ap_ratio<float>::num;++i)
            {
                const handle  rh(r);
                const natural nn = mul(rh,ten);
                q = quot(nn,den,r);
                ans += ap_ratio<float>::fac[i] * double( q.lsu() );
            }
            return ans;
        }

        template <>
        double natural:: ratio<double>(const natural &num, const natural &den)
        {

            uint_type    _10 = 10;
            const handle ten(_10);
            natural r;
            natural q   = quot(num,den,r);
            double  ans = q.to<double>();
            for(size_t i=0;i<ap_ratio<double>::num;++i)
            {
                const handle  rh(r);
                const natural nn = mul(rh,ten);
                q = quot(nn,den,r);
                ans += ap_ratio<double>::fac[i] * double( q.lsu() );
            }
            return ans;
        }

        template <>
        long double natural:: ratio<long double>(const natural &num, const natural &den)
        {

            uint_type    _10 = 10;
            const handle ten(_10);
            natural r;
            natural q   = quot(num,den,r);
            double  ans = q.to<long double>();
            for(size_t i=0;i<ap_ratio<long double>::num;++i)
            {
                const handle  rh(r);
                const natural nn = mul(rh,ten);
                q = quot(nn,den,r);
                ans += ap_ratio<long double>::fac[i] * (long double)( q.lsu() );
            }
            return ans;
        }

    }

}



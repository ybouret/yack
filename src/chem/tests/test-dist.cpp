#include "yack/utest/run.hpp"
#include "yack/counting/part.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sort/sum.hpp"
#include "yack/string/cxx-name.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

namespace {

    static inline
    void test(const readable<size_t> &nu, randomized::bits &ran)
    {
        std::cerr << std::endl;
        std::cerr << "<testing " << nu << ">" << std::endl;
        const size_t   n = nu.size();
        vector<double> num(n,as_capacity);
        vector<double> den(n,as_capacity);

        const size_t    N = 10000;
        vector<double>  res(N,as_capacity);
        for(size_t j=N;j>0;--j)
        {
            num.free();
            den.free();

            for(size_t i=n;i>0;--i)
            {
                double psi = ran.to<double>();
                num << fabs(psi*nu[i]);
                den << psi*psi;
            }

           res << squared( sorted::sum(num,sorted::by_value) )/sorted::sum(den,sorted::by_value);
        }
        hsort(res,comparison::increasing<double>);
        std::cerr << "\tin [" << res.front() << ":" << res.back() << "]" << std::endl;

        {
            string fn = "dist";
            for(size_t i=1;i<=n;++i)
            {
                fn += vformat("-%u",unsigned(nu[i]));
            }
            fn += ".dat";
            std::cerr << "\tto '" << fn << "'" << std::endl;
            ios::ocstream fp(fn);
            for(size_t i=1;i<=res.size();++i)
            {
                fp("%g %g\n", res[i], double(i)/N);
            }

        }

        std::cerr << "<testing " << nu << "/>" << std::endl;

    }
}

YACK_UTEST(dist)
{
    randomized::rand_ ran;

    partition part(3);

    part.boot();
    do
    {
        test(part,ran);
    }
    while(part.next());



}
YACK_UDONE()


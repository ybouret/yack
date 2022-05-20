#include "yack/apex/kernel.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

YACK_UTEST(apk)
{
    randomized::rand_ ran;

    vector<apq> data;

    for(size_t i=1;i<=4;++i)
    {
        data << apq(ran,8,8);
        std::cerr << "data=" << data << std::endl;
        const apn fac = apk::lcm(data);
        std::cerr << "|_factor=" << fac << std::endl;
        vector<apq> temp(data);
        for(size_t j=1;j<=temp.size();++j)
        {
            temp[j] *= fac;
        }
        std::cerr << "|_temp=" << temp << std::endl;
    }
    

}
YACK_UDONE()


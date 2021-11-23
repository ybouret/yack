
#include "yack/sort/sum.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;


YACK_UTEST(sort_sum)
{
    randomized::rand_ ran;
    const size_t      num = ran.leq(10);
    {
        vector<size_t>    seq(num,as_capacity);
        for(size_t i=1;i<=num;++i)
            seq.push_back(i);
        randomized::shuffle::data(seq(), seq.size(), ran);
        const size_t s = sorted::sum(seq,sorted::by_value);
        std::cerr << "sum(" << seq << ")=" << s << std::endl;
    }

    {
        vector<double> seq(num,as_capacity);
        for(size_t i=1;i<=num;++i)
            seq.push_back( ran.symm<double>() );
        const double s0 = sorted::sum(seq,sorted::by_value);
        std::cerr << "sum(" << seq << ")=" << s0 << std::endl;
        randomized::shuffle::data(seq(), seq.size(), ran);
        const double s1 = sorted::sum(seq,sorted::by_abs_value);
        std::cerr << "sum(" << seq << ")=" << s1 << std::endl;
    }



}
YACK_UDONE()


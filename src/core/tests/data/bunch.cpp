#include "yack/sequence/bunch.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

YACK_UTEST(data_bunch)
{
    randomized::rand_ ran;

    bunch<int>  ib(6);
    vector<int> toto(6);
    for(size_t i=1;i<=8;++i)
    {
        bring::fill(toto,ran);
        ib.ensure(toto);
        std::cerr << "ib=" << *ib << std::endl;
    }


}
YACK_UDONE()



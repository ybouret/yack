#include "yack/ipso/add.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"


using namespace yack;

template <typename T>
void perform(randomized::bits &ran)
{
    ipso::add<T> xadd;
    T            sum = 0;
    for(size_t i=ran.leq(10);i>0;--i)
    {
        const T tmp = bring::get<T>(ran);
        sum += tmp;
        xadd << tmp;
    }

    std::cerr << sum << " => " << *xadd << std::endl;

}

YACK_UTEST(ipso_add)
{
    randomized::rand_ ran;

    ipso::api<float>::display();
    ipso::api<double>::display();
    ipso::api<apq>::display();


    if(argc<=1)
    {
        perform<float>(ran);
        perform<double>(ran);
        perform<long double>(ran);
        perform<apq>(ran);
    }
    else
    {

    }
}
YACK_UDONE()


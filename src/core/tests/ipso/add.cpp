#include "yack/ipso/add.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"
#include "yack/ios/fmt/align.hpp"

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

    std::cerr << ios::align(rtti::name<T>(),ipso::api_::width,ios::align::left) << " : " << xadd << std::endl;
    std::cerr << ios::align(rtti::name<T>(),ipso::api_::width,ios::align::left) << " : " << sum << " => " << *xadd << std::endl;
    std::cerr << std::endl;
}

YACK_UTEST(ipso_add)
{
    randomized::rand_ ran;

    ipso::api<float>::display();
    ipso::api<double>::display();
    ipso::api<long double>::display();
    ipso::api<apq>::display();

    ipso::api< complex<float> >::display();
    ipso::api< complex<double> >::display();
    ipso::api< complex<long double> >::display();


    if(argc<=1)
    {
        perform<float>(ran);
        perform<double>(ran);
        perform<long double>(ran);
        perform< complex<float> >(ran);
        perform< complex<double> >(ran);
        perform< complex<long double> >(ran);
        perform<apq>(ran);
    }
    else
    {

    }

    YACK_SIZEOF( ipso::add<float>  );
    YACK_SIZEOF( ipso::add<double> );
    YACK_SIZEOF( ipso::add<apq>    );
    YACK_SIZEOF( ipso::add< complex<float> >    );

}
YACK_UDONE()


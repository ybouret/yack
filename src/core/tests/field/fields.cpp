#include "yack/field/in2d.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace  yack;


YACK_UTEST(fields)
{
    randomized::rand_ ran;

    field1D<string> s( "s", layout1D(-5,6) );

    for(unit_t i=s.lower;i<=s.upper;++i)
    {
        s[i] = bring::get<string>(ran);
    }

    for(unit_t i=s.lower;i<=s.upper;++i)
    {
        std::cerr << s[i] << std::endl;
    }


}
YACK_UDONE()

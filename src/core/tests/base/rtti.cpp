#include "yack/system/rtti.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(rtti)
{

    const rtti  &ti = rtti::use( typeid(char) );
    std::cerr << ti.name() << std::endl;
    rtti::use<char>( ti.name() );
    rtti::use<char>( "char" );
    std::cerr << ti.name()  << std::endl;
    std::cerr << ti << std::endl;
}
YACK_UDONE()



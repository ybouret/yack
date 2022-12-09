#include "yack/apex/north.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(apex_north)
{

    YACK_SIZEOF(north::qvector<int64_t>);
    YACK_SIZEOF(north::qvector<apq>);


    north::qvector<apq> toto(NULL,0);
    

}
YACK_UDONE()


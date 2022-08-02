#include "yack/associative/be-key.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_be_key)
{

    be_key<int> k1(0x1234);

    k1.display_hexa(std::cerr) << std::endl;


}
YACK_UDONE()


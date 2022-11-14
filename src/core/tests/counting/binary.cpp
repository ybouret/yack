#include "yack/utest/run.hpp"
#include "yack/counting/binary.hpp"

using namespace yack;

YACK_UTEST(counting_binary)
{
    YACK_SIZEOF(combination);
    YACK_SIZEOF(binary_spots);

    binary_spots b(4);
    
    do
    {
        std::cerr << b << std::endl;
    }
    while(b.next());

}
YACK_UDONE()

#include "yack/utest/run.hpp"
#include "yack/counting/binary.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

YACK_UTEST(counting_binary)
{
    YACK_SIZEOF(combination);
    YACK_SIZEOF(binary_spots);

    binary_spots b(4);
    vector<int>  u( b.capacity(), 2);

    
    do
    {
        b.fill(u);
        std::cerr << u << " <== " << b << std::endl;
    }
    while(b.next());

}
YACK_UDONE()

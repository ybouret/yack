#include "yack/utest/run.hpp"
#include "yack/counting/binary.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

YACK_UTEST(counting_binary)
{
    YACK_SIZEOF(combination);
    YACK_SIZEOF(binary_spots);


    for(size_t n=1;n<=6;++n)
    {
        std::cerr << "-------- n=" << n << " --------" << std::endl;

        binary_spots b(n);
        vector<int>  u( b.capacity(), 2);

        do
        {
            b.fill(u);
            std::cerr << u << " <== " << b << std::endl;
        }
        while(b.next());

        std::cerr << std::endl;
    }

}
YACK_UDONE()

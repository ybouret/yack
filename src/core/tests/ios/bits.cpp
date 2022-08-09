#include "yack/ios/bits.hpp"
#include "yack/utest/run.hpp"
#include "yack/arith/base2.hpp"

using namespace yack;

YACK_UTEST(ios_bits)
{
    randomized::rand_ ran;
    io_bits Q;

    Q.append<char>(0,0); YACK_ASSERT(0==Q.size);

    for(size_t iter=0;iter<10000;++iter)
    {
        const uint64_t i  = ran.to<uint64_t>();
        const size_t   nb = bits_for(i);
        Q.append(i,nb);
        YACK_ASSERT(Q.size==nb);
        const size_t j = Q.pluck<size_t>(nb);
        YACK_ASSERT(j==i);
        YACK_ASSERT(0==Q.size);

        Q.append(i);
        YACK_ASSERT(Q.size==sizeof(i)*8);
        const size_t k = Q.pluck<size_t>();
        YACK_ASSERT(k==i);
        YACK_ASSERT(0==Q.size);
    }

    {
        size_t i=0;
        for(size_t r=0;r<32;++r)
        {
            for(size_t c=0;c<8;++c)
            {
                Q.append(i,8);
                std::cerr << ' ' << Q;
                Q.release();
                ++i;
            }
            std::cerr << std::endl;
        }
    }
    std::cerr << std::endl;

    for(size_t i=0;i<20;++i)
    {
        const size_t   nb = bits_for(i);
        Q.append(i,nb);
        Q.flush();
        std::cerr << i << " => " << Q << std::endl;
        Q.release();
    }
    std::cerr << std::endl;


    Q("%s %u","Hello",7);
    std::cerr << "Q.size=" << Q.size << std::endl;
    std::cerr << Q << std::endl;





}
YACK_UDONE()

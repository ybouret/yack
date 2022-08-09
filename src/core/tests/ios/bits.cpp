#include "yack/ios/bits.hpp"
#include "yack/utest/run.hpp"
#include "yack/arith/base2.hpp"
#include "yack/apex/natural.hpp"
#include "yack/sequence/vector.hpp"

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
    std::cerr << std::endl;

    for(size_t iter=0;iter<10;++iter)
    {
        Q.release();
        const apn n(ran,1+ran.leq(255));
        n.output_hex(std::cerr << "n=0x") << std::endl;
        Q.add(n);
        YACK_ASSERT(Q.size==n.bits());
        Q.fuzz(ran);
        std::cerr << "Q=" << Q << std::endl;
        const apn q = Q.ap(n.bits());
        q.output_hex(std::cerr << "q=0x") << std::endl;
        YACK_ASSERT(n==q);
        std::cerr << std::endl;
    }

    {
        Q.release();
        vector<apn> vn;
        for(size_t iter=5+ran.leq(10);iter>0;--iter)
        {
            const apn n(ran,1+ran.leq(60));
            vn.push_back(n);
            Q.add(n);
            Q.fuzz(ran);
            std::cerr << " -> " << n << std::endl;
        }
        std::cerr << "Q=" << Q << std::endl;
        for(size_t i=1;i<=vn.size();++i)
        {
            const apn &n = vn[i];
            const apn  q = Q.ap(n.bits());
            YACK_ASSERT(q==n);
            Q.drop();
            std::cerr << " <- " << q << std::endl;
        }
    }




}
YACK_UDONE()

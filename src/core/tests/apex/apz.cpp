
#include "yack/apex/integer.hpp"
#include "yack/utest/run.hpp"
#include "yack/randomized/rand.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

YACK_UTEST(apz)
{
    randomized::rand_ ran;

    const int64_t add_max = 1000000000;
    const int64_t add_min = -add_max;

    std::cerr << "[ADD/SUB]" << std::endl;
    for(size_t iter=0;iter<=1024*16;++iter)
    {
        const int64_t l = ran.in(add_min,add_max);
        const int64_t r = ran.in(add_min,add_max);
        const apz     L = l, LL=l;
        const apz     R = r, RR=r;
        YACK_ASSERT(L==l);
        YACK_ASSERT(l==L);
        YACK_ASSERT(LL==L);
        YACK_ASSERT(R==r);
        YACK_ASSERT(r==R);
        YACK_ASSERT(RR==R);

        const int64_t s = l+r;
        { const apz S = L+R; YACK_ASSERT(S==s); }
        { const apz S = l+R; YACK_ASSERT(S==s); }
        { const apz S = L+r; YACK_ASSERT(S==s); }
        { apz S = L; S += R; YACK_ASSERT(S==s); }
        { apz S = L; S += r; YACK_ASSERT(S==s); }

        const int64_t d = l-r;
        { const apz D = L-R; YACK_ASSERT(D==d); }
        { const apz D = l-R; YACK_ASSERT(D==d); }
        { const apz D = L-r; YACK_ASSERT(D==d); }
        { apz D = L; D -= R; YACK_ASSERT(D==d); }
        { apz D = L; D -= r; YACK_ASSERT(D==d); }
    }

    std::cerr << "[MUL/DIV]" << std::endl;
    const int64_t mul_max = 65000;
    const int64_t mul_min = -mul_max;
    for(size_t iter=0;iter<=1024*16;++iter)
    {
        const int64_t l = ran.in(mul_min,mul_max);
        int64_t       r = ran.in(mul_min,mul_max);
        while(0==r)   r = ran.in(mul_min,mul_max);
        const apz     L = l;
        const apz     R = r;

        const int64_t p = l*r;

        { const apz P = L*R; YACK_ASSERT(P==p); }
        { const apz P = l*R; YACK_ASSERT(P==p); }
        { const apz P = L*r; YACK_ASSERT(P==p); }
        { apz P = L; P *= R; YACK_ASSERT(P==p); }
        { apz P = L; P *= r; YACK_ASSERT(P==p); }

    }

    std::cerr << "[INCR/DECR]" << std::endl;
    for(apz i=-10;i<=10;)
    {
        std::cerr << ++i << '/';
    }
    std::cerr << std::endl;
    
    for(apz i=-10;i<=10;)
    {
        std::cerr << i++ << '/';
    }
    std::cerr << std::endl;

    for(apz i=10;i>=-10;)
    {
        std::cerr << i-- << '/';
    }
    std::cerr << std::endl;

    for(apz i=10;i>=-10;)
    {
        std::cerr << --i << '/';
    }
    std::cerr << std::endl;

}
YACK_UDONE()

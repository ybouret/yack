
#include "yack/apex.hpp"
#include "yack/utest/run.hpp"
#include "yack/randomized/rand.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

template <typename T> static inline void check_cmp(const apq &q, randomized::bits &)
{
    const T one = 1;
    const T two = 2;
    YACK_CHECK(q!=one);
    YACK_CHECK(one!=q);
    YACK_CHECK(two!=q);
    YACK_CHECK(!(q==one));
    YACK_CHECK(!(q==two));
    YACK_CHECK(!(one==q));
    YACK_CHECK(!(two==q));
    YACK_CHECK(one<q);
    YACK_CHECK(one<=q);
    YACK_CHECK(q<=two);
    YACK_CHECK(q<two);
}

#define YACK_APQ_CHECK(PROC) \
PROC<apex::rational>(q,ran);\
PROC<apex::int_type>(q,ran);\
PROC<apex::integer>(q,ran);\
PROC<apex::natural>(q,ran);

template <typename T> static inline void check_add(const apq &q, randomized::bits &ran)
{
    const T   t   = apex::int_type(ran.in(-1000,1000));
    const apq Q   = t;
    const apq sum = q+Q;
    YACK_ASSERT(q+t==sum);
    YACK_ASSERT(t+q==sum);
    {
        apq tmp=q;
        tmp += t;
        YACK_ASSERT(tmp==sum);
    }
}

template <typename T> static inline void check_sub(const apq &q, randomized::bits &ran)
{
    const T   t   = apex::int_type(ran.in(-1000,1000));
    const apq Q   = t;
    const apq sum = q-Q;
    YACK_ASSERT(q-t==sum);
    YACK_ASSERT(t-q==-sum);
    {
        apq tmp=q;
        tmp -= t;
        YACK_ASSERT(tmp==sum);
    }
}


YACK_UTEST(apq)
{
    randomized::rand_ ran;

    { apq q; std::cerr << q << std::endl; }
    { apq q = 7; std::cerr << q << std::endl; }
    { apq q(5,10); std::cerr << q << std::endl; }

    std::cerr << "[CMP]" << std::endl;
    {
        const apq q(5,3);
        std::cerr << "q=" << q << std::endl;
        YACK_APQ_CHECK(check_cmp);
    }

    std::cerr << "[ADD]" << std::endl;
    for(size_t iter=0;iter<1024;++iter)
    {
        const apq q(ran,20,8);
        {
            const apq qq = +q;
            YACK_ASSERT(qq==q);
        }
        YACK_APQ_CHECK(check_add);
    }

    for(apq q(-2,3);q<=10;)
    {
        std::cerr << ++q << '/';
    }
    std::cerr << std::endl;

    for(apq q(-2,3);q<=10;)
    {
        std::cerr << q++ << '/';
    }
    std::cerr << std::endl;

    std::cerr << "[SUB]" << std::endl;
    for(size_t iter=0;iter<1024;++iter)
    {
        const apq q(ran,20,8);
        {
            const apq m = -q;
            YACK_ASSERT(0 == m+q );
        }
        YACK_APQ_CHECK(check_sub);
    }

    for(apq q(2,3);q>=-10;)
    {
        std::cerr << --q << '/';
    }
    std::cerr << std::endl;

    for(apq q(2,3);q>=-10;)
    {
        std::cerr << q-- << '/';
    }
    std::cerr << std::endl;

}
YACK_UDONE()



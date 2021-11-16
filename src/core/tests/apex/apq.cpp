
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
    const apq dif = q-Q;
    YACK_ASSERT(q-t==dif);
    YACK_ASSERT(t-q==-dif);
    {
        apq tmp=q;
        tmp -= t;
        YACK_ASSERT(tmp==dif);
    }
}

template <typename T> static inline void check_mul(const apq &q, randomized::bits &ran)
{
    const T   t   = apex::int_type(ran.in(-1000,1000));
    const apq Q   = t;
    const apq prod = q*Q;
    YACK_ASSERT(q*t==prod);
    YACK_ASSERT(t*q==prod);
    {
        apq tmp=q;
        tmp *= t;
        YACK_ASSERT(tmp==prod);
    }
}

template <typename T> static inline void check_div(const apq &q, randomized::bits &ran)
{
    T            t    = apex::int_type(ran.in(-1000,1000));
    while(t==0)  t    = apex::int_type(ran.in(-1000,1000));
    const apq    Q    = t;
    const apq    quot = q/Q;
    const apq    invq = Q/q;
    YACK_ASSERT(q/t==quot);
    YACK_ASSERT(t/q==invq);
    {
        apq tmp=q;
        tmp /= t;
        YACK_ASSERT(tmp==quot);
    }
}

#include "yack/sequence/vector.hpp"

static void test_large(randomized::bits &ran)
{
    const size_t n = 10 + ran.leq(200);

    vector<apq>  Q(n,as_capacity);
    for(size_t i=n;i>0;--i) Q.push_back( apq(ran,30,10) );
    std::cerr << "With #" << n << " apq" << std::endl;
    std::cerr << "|_add/sub" << std::endl;
    {
        apq sum = 0;
        for(size_t i=n;i>0;--i) sum += Q[i];
        randomized::shuffle::data(&Q[1],n,ran);
        for(size_t i=n;i>0;--i) sum -= Q[i];
        YACK_CHECK(0==sum);
    }

    std::cerr << "|_mul/div" << std::endl;
    {
        apq fac = 1;
        randomized::shuffle::data(&Q[1],n,ran);
        for(size_t i=n;i>0;--i) fac *= Q[i];
        randomized::shuffle::data(&Q[1],n,ran);
        for(size_t i=n;i>0;--i) fac /= Q[i];
        YACK_CHECK(1==fac);

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

    std::cerr << "[MUL]" << std::endl;
    for(size_t iter=0;iter<256;++iter)
    {
        const apq q(ran,20,8);
        YACK_APQ_CHECK(check_mul);
    }

    std::cerr << "[DIV]" << std::endl;
    for(size_t iter=0;iter<256;++iter)
    {
        const apq q(ran,20,8);
        YACK_APQ_CHECK(check_div);
    }

    std::cerr << "[Large]" << std::endl;
    test_large(ran);

}
YACK_UDONE()



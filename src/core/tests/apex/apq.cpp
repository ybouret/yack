
#include "yack/apex.hpp"
#include "yack/utest/run.hpp"
#include "yack/randomized/rand.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

template <typename T> static inline void check_cmp(const apq &q)
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
PROC<apex::rational>(q);\
PROC<apex::int_type>(q);\
PROC<apex::integer>(q);\
PROC<apex::natural>(q);

template <typename T> static inline void check_add(const apq &q)
{
    const apq q1  = 1;
    const T   one = 1;
    const apq sum = q+q1;
    YACK_ASSERT(q+one==sum);
    YACK_ASSERT(one+q==sum);
    {
        apq tmp=q;
        tmp += one;
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

}
YACK_UDONE()



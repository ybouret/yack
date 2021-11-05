
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



YACK_UTEST(apq)
{
    randomized::rand_ ran;

    { apq q; std::cerr << q << std::endl; }
    { apq q = 7; std::cerr << q << std::endl; }
    { apq q(5,10); std::cerr << q << std::endl; }

    {
        const apq q(5,3);
        std::cerr << "q=" << q << std::endl;
        check_cmp<apex::int_type>(q);
        check_cmp<apex::integer>(q);
        check_cmp<apex::natural>(q);
    }

}
YACK_UDONE()



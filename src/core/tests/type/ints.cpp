
#include "yack/type/ints.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

namespace
{
    template <typename U>
    static inline bool CheckIntegralFor() throw()
    {
        typedef typename integral_for<U>::type V;
        return (sizeof(V) == sizeof(U)) && ( is_signed<U>::value == is_signed<V>::value );
    }
}


YACK_UTEST(type_ints)
{
    YACK_CHECK(sizeof(unsigned_int<1>::type)==1);
    YACK_CHECK(sizeof(unsigned_int<2>::type)==2);
    YACK_CHECK(sizeof(unsigned_int<4>::type)==4);
    YACK_CHECK(sizeof(unsigned_int<8>::type)==8);

    YACK_CHECK(sizeof(signed_int<1>::type)==1);
    YACK_CHECK(sizeof(signed_int<2>::type)==2);
    YACK_CHECK(sizeof(signed_int<4>::type)==4);
    YACK_CHECK(sizeof(signed_int<8>::type)==8);

    
    YACK_CHECK(is_signed<int>::value);
    YACK_CHECK(!is_signed<unsigned>::value);

    YACK_CHECK(CheckIntegralFor<int>());
    YACK_CHECK(CheckIntegralFor<unsigned>());

}
YACK_UDONE()


#include "yack/apex.hpp"
#include "yack/apex/primes.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(apex_info)
{
    std::cerr << "size of apex classes:" << std::endl;
    YACK_SIZEOF(apn);
    YACK_SIZEOF(apz);
    YACK_SIZEOF(apq);
    YACK_SIZEOF(apex::prime_knot);
    std::cerr << std::endl;

    std::cerr << "apex::natural fields:" << std::endl;
    YACK_OUTPUT(apn::core_size);
    YACK_OUTPUT(apn::word_size);
    YACK_OUTPUT(apn::min_words_bytes);


}
YACK_UDONE()


#include "yack/ios/scribe/repository.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_scribe)
{

    ios::scribes &repo = ios::scribes::instance();
    std::cerr << repo.call_sign << std::endl;

}
YACK_UDONE()

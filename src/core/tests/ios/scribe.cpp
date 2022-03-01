
#include "yack/ios/scribe/repository.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_scribe)
{

    ios::scribes &repo = ios::scribes::instance();
    std::cerr << repo.call_sign << std::endl;


    {
        char c = 'A';
        std::cerr << repo(c) << std::endl;
    }

    {
        const float f =  1.01f;
        std::cerr << repo(f) << std::endl;

    }


    {
        const double f =  1.01;
        std::cerr << repo(f) << std::endl;

    }

    {
        const long double f =  1.01;
        std::cerr << repo(f) << std::endl;

    }


}
YACK_UDONE()


#include "yack/ios/markup.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"

using namespace yack;

YACK_UTEST(ios_markup)
{
    bool verbose = !environment::flag("SILENT");
    {
        ios::markup m1(verbose,std::cerr,"//  ","main");
        {
            ios::markup m2(verbose,std::cerr,"//    ","sub");

        }
    }

}
YACK_UDONE()


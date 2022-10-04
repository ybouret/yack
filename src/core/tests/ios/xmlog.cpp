#include "yack/ios/xmlog.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_xmlog)
{
    bool verbose = true;

    xmlog lf("[xmlog]",std::cerr,verbose);

    std::cerr << "Prefix: " << lf.prefix << std::endl;
    *lf << "Top Level" << std::endl;
    {
        const xmlog::msg m1(lf,"data","");
        *lf << "within data..." << std::endl;
        {
            const xmlog::msg m2(lf,"sub", vformat("#%u",4) );
            *lf << "within sub" << std::endl;
        }
    }

}
YACK_UDONE()


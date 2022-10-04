#include "yack/ios/xmlog.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"

using namespace yack;

YACK_UTEST(ios_xmlog)
{
    bool verbose = environment::flag("VERBOSE");

    xmlog lf("[xmlog]",std::cerr,verbose);

    std::cerr << "Prefix: " << lf.prefix << std::endl;
    YACK_XMLOG_PRINTLN(lf,"Top Level");
    {
        const xmlog::msg m1(lf,"data","");
        YACK_XMLOG_PRINTLN(lf, "within data...");
        {
            const xmlog::msg m2(lf,"sub", vformat("#%u",4) );
            YACK_XMLOG_PRINTLN(lf, "within sub");
        }
    }

    YACK_XMLSUB(lf,"Top");
    YACK_XMLSUB_OPT(lf,"Sub1",vformat(" N='%u'",4));
    YACK_XMLOUT(lf,"Sub2");

}
YACK_UDONE()


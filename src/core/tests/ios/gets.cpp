#include "yack/ios/icstream.hpp"
#include "yack/utest/run.hpp"


using namespace yack;

YACK_UTEST(ios_gets)
{

    if(argc>1)
    {
        ios::icstream   fp(argv[1]);
        ios::characters line;

        while(fp.gets(line))
        {
            std::cout << line << std::endl;
        }

    }

}
YACK_UDONE()



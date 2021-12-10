#include "yack/jive/pattern/matching.hpp"
#include "yack/utest/run.hpp"
#include "yack/jive/pattern/posix.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;

YACK_UTEST(matching)
{

    if(argc>1)
    {
        const string         expr = argv[1];
        const jive::matching match = expr;

        if(argc>2)
        {
            ios::icstream   fp(argv[2]);
            ios::characters line;
            while(fp.gets(line))
            {
                const string data = line.to_string();
                std::cerr << data << std::endl;
            }

        }

    }

}
YACK_UDONE()


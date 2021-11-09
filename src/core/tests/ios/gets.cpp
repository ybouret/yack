#include "yack/ios/icstream.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/small/zcache.hpp"


using namespace yack;

YACK_UTEST(ios_gets)
{

    memory::zcache &repo = ios::character::provider();
    repo.load(1000);

    if(argc>1)
    {
        ios::icstream   fp(argv[1]);
        ios::characters line;

        while(fp.gets(line))
        {
            char *msg = line.cstr();
            std::cout << line << "/" << msg << std::endl;
            line.free(msg);
        }

    }

    std::cerr << "#repo=" << repo.here() << std::endl;

}
YACK_UDONE()



#include "yack/hashing/perfect.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/string.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

YACK_UTEST(hashing_perfect)
{
    hashing::perfect mph;
    vector<string>   keys;

    std::cerr << "<reading>" << std::endl;
    if(argc>1)
    {
        ios::icstream   fp(argv[1]);
        ios::characters line;
        while(fp.gets(line))
        {
            const string key = line.to_string();
            mph  << key;
            keys << key;
        }
    }

    std::cerr << "<checking>" << std::endl;
    {
        int hash = 0;
        for(size_t i=1;i<=keys.size();++i,++hash)
        {
            YACK_ASSERT(mph(keys[i])==hash);
        }
    }


    mph.gv("mph.dot");

}
YACK_UDONE()


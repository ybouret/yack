#include "yack/system/env.hpp"
#include "yack/associative/hash/map.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(env)
{
    std::cerr << "ENV" << std::endl;
	environment::show();
    std::cerr << std::endl;

    std::cerr << "ENV db" << std::endl;
    hash_map<string, string> db;
	environment::get(db);
    std::cerr << db << std::endl;
    std::cerr << std::endl;

    std::cerr << "User's queries..." << std::endl;
    for(int i=1;i<argc;++i)
    {
        string val;
        const char *key = argv[i];
        if( environment::get(key,&val) )
        {
            std::cerr << key << " = " << val << std::endl;
        }
        else
        {
            std::cerr << "NO " << key << std::endl;
        }
    }

}
YACK_UDONE()


#include "yack/json/parser.hpp"
#include "yack/json/value.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(parser)
{
    JSON::Value  v1 = true;
    JSON::Value  v2 = false;
    JSON::Value  v3 = 0.0;
    JSON::Value  v4 = "hello";
    const string s  = "world";
    JSON::Value  v5 = s;
    JSON::Value  v6 = JSON::asArray;

    YACK_SIZEOF(JSON::Array);
    YACK_SIZEOF(JSON::Object);


    JSON::Parser J;

    if(argc>1)
    {
        const JSON::Value &jv = J(jive::module::open_file(argv[1]));
        std::cerr << jv << std::endl;
    }

}
YACK_UDONE()

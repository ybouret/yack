
#include "yack/jive/tags.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace jive;

YACK_UTEST(tags)
{
    tags &db = tags::instance();
    std::cerr << db << std::endl;

    tags::make("hello");
    tags::make("hello",4);
    tags::make('w');
    {
        const string key = "world";
        tags::make(key);
    }

    std::cerr << db << std::endl;

    db.gv();

}
YACK_UDONE()


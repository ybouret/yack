#include "yack/jive/pattern/all.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;

YACK_UTEST(pattern)
{
    std::cerr << "create/save" << std::endl;
    jive::single p('a');
    jive::within q('a','z');
    {
        ios::ocstream fp("patterns.bin");
        p.serialize(fp);
        q.serialize(fp);
    }

    std::cerr << "reload" << std::endl;
    {
        ios::icstream          fp("patterns.bin");
        {
            arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
            YACK_CHECK(jive::single::mark==mp->uuid);
            YACK_CHECK('a'==mp->as<jive::single>()->code);
        }
        {
            arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
            YACK_CHECK(jive::within::mark==mp->uuid);
            YACK_CHECK('a'==mp->as<jive::within>()->lower);
            YACK_CHECK('z'==mp->as<jive::within>()->upper);
        }

    }
}
YACK_UDONE()

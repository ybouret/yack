#include "yack/jive/pattern/all.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;

YACK_UTEST(pattern)
{
    YACK_SIZEOF(jive::single);
    YACK_SIZEOF(jive::within);
    YACK_SIZEOF(jive::joker);
    YACK_SIZEOF(jive::optional);
    YACK_SIZEOF(jive::at_least);


    std::cerr << "create/save" << std::endl;
    jive::single p0('a');
    jive::within p1('a','z');
    auto_ptr<jive::pattern> p2 = jive::optional::create( new jive::single('b') );
    auto_ptr<jive::pattern> p3 = jive::at_least::create( 1, new jive::single('c') );
    auto_ptr<jive::pattern> p4 = jive::counting::create( 2, 3, new jive::single('d') );
    auto_ptr<jive::pattern> p5 = jive::zero_or_more( new jive::within('A','Z') );
    auto_ptr<jive::pattern> p6 = jive::one_or_more( new jive::within('0','9') );


    {
        ios::ocstream fp("patterns.bin");
        p0.serialize(fp);
        p1.serialize(fp);
        p2->serialize(fp);
        p3->serialize(fp);
        p4->serialize(fp);
        p5->serialize(fp);
        p6->serialize(fp);

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
        
        {
            arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
            YACK_CHECK(jive::optional::mark==mp->uuid);

        }

        {
            arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
            YACK_CHECK(jive::at_least::mark==mp->uuid);
            YACK_CHECK(1==mp->as<jive::at_least>()->count);
        }

        {
            arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
            YACK_CHECK(jive::counting::mark==mp->uuid);
            YACK_CHECK(2==mp->as<jive::counting>()->nmin);
            YACK_CHECK(3==mp->as<jive::counting>()->nmax);

        }



    }
}
YACK_UDONE()

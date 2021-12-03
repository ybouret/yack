#include "yack/jive/pattern/all.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;

YACK_UTEST(pattern)
{
    {
        std::cerr << "<basic>" << std::endl;
        YACK_SIZEOF(jive::single);
        YACK_SIZEOF(jive::within);
        YACK_SIZEOF(jive::except);
        std::cerr << "<basic/>" << std::endl << std::endl;

        std::cerr << "<joker>" << std::endl;
        YACK_SIZEOF(jive::optional);
        YACK_SIZEOF(jive::at_least);
        YACK_SIZEOF(jive::counting);
        std::cerr << "<joker/>" << std::endl << std::endl;


        std::cerr << "<logic>" << std::endl;
        YACK_SIZEOF(jive::op_and);
        YACK_SIZEOF(jive::op_or);
        YACK_SIZEOF(jive::op_none);
        std::cerr << "<logic/>" << std::endl << std::endl;

        YACK_SIZEOF(jive::motif);


        std::cerr << "create/save" << std::endl;
        jive::single p0('a');
        jive::within p1('a','z');
        auto_ptr<jive::pattern> p2 = jive::optional::create( new jive::single('b') );
        auto_ptr<jive::pattern> p3 = jive::at_least::create( 1, new jive::single('c') );
        auto_ptr<jive::pattern> p4 = jive::counting::create( 2, 3, new jive::single('d') );
        auto_ptr<jive::pattern> p5 = jive::zero_or_more( new jive::within('A','Z') );
        auto_ptr<jive::pattern> p6 = jive::one_or_more(  new jive::within('0','9') );

        auto_ptr<jive::op_and> p7 = new jive::op_and();
        {
            *p7 << new jive::single('a');
            *p7 << jive::optional::create( new jive::single('b') );
        }

        auto_ptr<jive::op_or> p8 = new jive::op_or();
        {
            *p8 << new jive::within('a','z');
            *p8 << jive::optional::create( new jive::within('0','9') );
        }

        auto_ptr<jive::op_none> p9 = new jive::op_none();
        {
            *p9 << new jive::except(' ');
        }


        {
            ios::ocstream fp("patterns.bin");
            p0.serialize(fp);
            p1.serialize(fp);
            p2->serialize(fp);
            p3->serialize(fp);
            p4->serialize(fp);
            p5->serialize(fp);
            p6->serialize(fp);
            p7->serialize(fp);
            p8->serialize(fp);
            p9->serialize(fp);

        }

        std::cerr << "reload" << std::endl;
        {
            ios::icstream          fp("patterns.bin");
            {
                arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
                YACK_CHECK(*mp==p0);

            }
            {
                arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
                YACK_CHECK(*mp==p1);
            }

            {
                arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
                YACK_CHECK(*mp==*p2);

            }

            {
                arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
                YACK_CHECK(*mp==*p3);

            }

            {
                arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
                YACK_CHECK(*mp==*p4);
            }


            {
                arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
                YACK_CHECK(*mp==*p5);
            }

            {
                arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
                YACK_CHECK(*mp==*p6);
            }

            {
                arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
                YACK_CHECK(*mp==*p7);
            }

            {
                arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
                YACK_CHECK(*mp==*p8);
            }

            {
                arc_ptr<jive::pattern> mp = jive::pattern::load(fp);
                YACK_CHECK(*mp==*p9);
            }

        }
    }

    

}
YACK_UDONE()

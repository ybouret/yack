
#include "yack/jive/pattern/all.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;

namespace
{
    static inline void render(const jive::pattern &p, const char *filename)
    {
        p.makeGV(filename);
    }
}


YACK_UTEST(viz)
{
    {
        auto_ptr<jive::pattern> p = new jive::single('a');
        render(*p,"single.dot");
    }

    {
        auto_ptr<jive::pattern> p = new jive::within('a','z');
        render(*p,"within.dot");
    }

    {
        auto_ptr<jive::pattern> p = new jive::except('b');
        render(*p,"except.dot");
    }

    {
        auto_ptr<jive::pattern> p = jive::optional::create( new jive::single('c') );
        render(*p,"optional.dot");
    }

    {
        auto_ptr<jive::pattern> p = jive::at_least::create(0,new jive::single('d') );
        render(*p,"zom.dot");
    }

    {
        auto_ptr<jive::pattern> p = jive::at_least::create(1,new jive::single('e') );
        render(*p,"oom.dot");
    }

    {
        auto_ptr<jive::pattern> p = jive::at_least::create(2,new jive::single('f') );
        render(*p,"at-least.dot");
    }

    {
        auto_ptr<jive::pattern> p = jive::counting::create(0,4,new jive::single('g') );
        render(*p,"counting.dot");
    }

    {
        auto_ptr<jive::logical> p = new jive::op_and();
        (*p) << new jive::single('a');
        render(*p,"and1.dot");
        (*p) << new jive::single('b');
        render(*p,"and2.dot");
    }

    {
        auto_ptr<jive::logical> p = new jive::op_or();
        (*p) << new jive::single('a');
        render(*p,"or1.dot");
        (*p) << new jive::single('b');
        render(*p,"or2.dot");
    }

    {
        auto_ptr<jive::logical> p = new jive::op_none();
        (*p) << new jive::single('a');
        render(*p,"none1.dot");
        (*p) << new jive::single('b');
        render(*p,"none2.dot");
    }


}
YACK_UDONE()

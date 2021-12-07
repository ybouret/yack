#include "yack/jive/pattern/regexp.hpp"
#include "yack/jive/pattern/dictionary.hpp"
#include "yack/utest/run.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/jive/pattern/posix.hpp"

using namespace yack;

YACK_UTEST(regexp)
{
    jive::dictionary dict;
    dict("INT", jive::one_or_more( jive::posix::digit() ) );
    dict.query("INT")->makeGV("int.dot");

    jive::pattern::verbose = true;
    if(argc>1)
    {
        const string      rs = argv[1];
        const jive::motif rx = jive::regexp::compile(rs,&dict);
        rx->makeGV("rx.dot");
        
    }

}
YACK_UDONE()


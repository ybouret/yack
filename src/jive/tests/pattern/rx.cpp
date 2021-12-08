#include "yack/jive/pattern/scatter-table.hpp"
#include "yack/jive/pattern/regexp.hpp"
#include "yack/jive/pattern/dictionary.hpp"
#include "yack/utest/run.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/jive/pattern/posix.hpp"
#include "yack/jive/pattern/first-bytes.hpp"

using namespace yack;

YACK_UTEST(regexp)
{

    jive::dictionary dict;
    dict("INT", jive::one_or_more( jive::posix::digit() ) );
    dict.query("INT")->makeGV("int.dot");

    jive::scatter_table lut;


    //jive::pattern::verbose = true;
    jive::patterns motifs;
    if(argc>1)
    {
        const string      rs = argv[1];
        std::cerr << "Expression: '" << rs << "'" << std::endl;
        const jive::pattern *rx = motifs.push_back( jive::regexp::compile(rs,&dict) );
        rx->makeGV("rx.dot");
        jive::first_bytes fc;
        rx->firsts(fc);
        std::cerr << "first bytes : " << fc << std::endl;
        std::cerr << "strong      : " << rx->strong() << std::endl;
        const string express = rx->express();
        std::cerr << "express     : " << express << std::endl;
        const jive::motif ry = jive::regexp::compile(express);
        YACK_CHECK(*rx==*ry);
        lut(*rx);
    }

    YACK_SIZEOF(jive::first_bytes);
    
}
YACK_UDONE()


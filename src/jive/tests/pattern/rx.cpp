#include "yack/jive/pattern/scatter-table.hpp"
#include "yack/jive/pattern/regexp.hpp"
#include "yack/jive/pattern/dictionary.hpp"
#include "yack/utest/run.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/jive/pattern/posix.hpp"
#include "yack/jive/pattern/first-bytes.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include "yack/ios/serializer/cfile.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;

YACK_UTEST(regexp)
{

    jive::dictionary dict;
    dict("INT", jive::one_or_more( jive::posix::digit() ) );
    dict.query("INT")->makeGV("int.dot");

    jive::scatter::table lut;


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
        ios::serializer::cfile::save("rx.bin",*rx);
        std::cerr << "first bytes : " << fc << std::endl;
        std::cerr << "strong      : " << rx->strong() << std::endl;
        const string express = rx->express();
        std::cerr << "express     : " << express << std::endl;
        const jive::motif expressed = jive::regexp::compile(express);
        ios::serializer::cfile::save("xp.bin",*expressed);
        expressed->makeGV("xp.dot");

        YACK_CHECK(*rx==*expressed);
        {
            ios::icstream fp("rx.bin");
            const jive::motif rz = jive::pattern::load(fp);
            YACK_CHECK(*rz==*rx);
        }
        lut.store(*rx,NULL);
        std::cerr << lut << std::endl;

    }
    

}
YACK_UDONE()


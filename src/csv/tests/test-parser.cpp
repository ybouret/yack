
#include "yack/csv/csv.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include "yack/ios/ascii/encoded.hpp"

using namespace yack;


YACK_UTEST(parser)
{

    if(false)
    {
        vector<uint8_t> ok;
        for(int i=32;i<128;++i)
        {
            const char c = char(i);
            switch(c)
            {
                case '\"':
                case ',' :
                    continue;

                default:
                    ok << c;
            }
        }
        //std::cerr << "ok=" << ok << std::endl;
        std::cerr << std::endl;

        std::cerr << "static const char CSV_RawChars[" << ok.size()+1 << "]={" << std::endl;

        for(size_t i=1; i <= ok.size(); ++i)
        {
            std::cerr << "'" << ios::ascii::encoded[ok[i]] << "'";
            std::cerr << ",";
            if( 0==(i & 7) ) std::cerr << std::endl; else std::cerr << ' ';
        }
        std::cerr << 0;


        std::cerr << std::endl;
        std::cerr << "};" << std::endl;
        std::cerr << std::endl;

    }

    CSV::Parser    csv;
    vector<string> terminals;
    vector<string> internals;
    csv.collect_keywords(terminals,internals);

    std::cerr << "terminals : " << terminals << std::endl;
    std::cerr << "internals : " << internals << std::endl;

    {
        ios::ocstream fp(ios::cstderr);
        jive::syntax::grammar::emit_keywords("CSV_",fp,terminals,"terminals");
    }

    {
        ios::ocstream fp(ios::cstderr);
        jive::syntax::grammar::emit_keywords("CSV_",fp,internals,"internals");
    }



    if(argc>1)
    {
        auto_ptr<CSV::Document> doc = csv( jive::module::open_file(argv[1]) );
        std::cerr << "#line=" << doc->size << std::endl;

        std::cerr << doc << std::endl;

    }



}
YACK_UDONE()

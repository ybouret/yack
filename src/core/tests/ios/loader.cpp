#include "yack/ios/ascii/converter.hpp"
#include "yack/ios/ascii/loader.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/sequence/list.hpp"
#include "yack/ios/icstream.hpp"

using namespace yack;

YACK_UTEST(ios_loader)
{
    ios::ascii::converter &cnv = ios::ascii::converter::instance();
    std::cerr << cnv.call_sign << std::endl;

    YACK_SIZEOF(ios::ascii::loader::column);

    ios::ascii::loader ld;
    vector<string>     a;
    list<string>       b;
    vector<int>        c;
    vector<float>      d;

    ld(a,1);
    ld(b,1);
    ld(c,1);
    ld(d,2);

    if(argc>1)
    {
        ios::icstream fp(argv[1]);

        const size_t nr = ld.load(fp);
        std::cerr << "#nr=" << nr << std::endl;
        std::cerr << "a=" << a << std::endl;
        std::cerr << "b=" << b << std::endl;
        std::cerr << "c=" << c << std::endl;
        std::cerr << "d=" << d << std::endl;

    }

}
YACK_UDONE()

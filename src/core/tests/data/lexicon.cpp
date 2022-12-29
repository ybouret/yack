
#include "yack/associative/lexicon.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_lexicon)
{
    lexicon<int> toto;

    toto.ensure(0x01);
    toto.ensure(0x02);
    toto.ensure(0x03);
    toto.ensure(0x0100);
    toto.ensure(0x0102);


    toto.gv("lexicon.dot");
    ios::vizible::render("lexicon.dot");

    for(lexicon<int>::const_iterator it=toto.begin();it!=toto.end();++it)
    {
        std::cerr << *it << std::endl;
    }

}
YACK_UDONE()



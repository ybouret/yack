
#include "yack/jive/pattern/all.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

YACK_UTEST(spec)
{
    ios::ocstream afp("among.bin");
    ios::ocstream efp("equal.bin");
    ios::ocstream xfp("avoid.bin");

    for(int i=1;i<argc;++i)
    {
        std::cerr << "Using '" << argv[i] << "'" << std::endl;
        const jive::motif p = jive::logical::among(argv[i]);
        const jive::motif q = jive::logical::equal(argv[i]);
        const jive::motif r = jive::logical::avoid(argv[i]);

        p->serialize(afp);
        q->serialize(efp);
        r->serialize(xfp);

        {
            p->makeGV("among.dot");
            q->makeGV("equal.dot");
            q->makeGV("avoid.dot");
        }
    }
}
YACK_UDONE()

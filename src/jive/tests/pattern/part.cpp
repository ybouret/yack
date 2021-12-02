#include "yack/jive/pattern/partition.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(part)
{
    YACK_SIZEOF(jive::domain);

    jive::partition p;
    std::cerr << p << std::endl;
    p.add('b');
    std::cerr << p << std::endl;
    p.add('b');
    std::cerr << p << std::endl;
    p.add('a');
    std::cerr << p << std::endl;
    p.add('c','f');
    std::cerr << p << std::endl;
    p.add('z');
    std::cerr << p << std::endl;
}
YACK_UDONE()


#include "yack/info/compression/alphabet.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace information;

YACK_UTEST(info_alphabet)
{

    YACK_SIZEOF(alphabet::node_t);
    std::cerr << "required: " << alphabet::space << " bytes" << std::endl;
    YACK_SIZEOF(alphabet);

    alphabet alpha;

}
YACK_UDONE()


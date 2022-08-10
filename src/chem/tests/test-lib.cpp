#include "yack/chem/library.hpp"
#include "yack/chem/designer.hpp"

#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;


YACK_UTEST(lib)
{
    library lib;

    lib("H+",1);
    lib("HO-",-1);



    const library lcp(lib);

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "lcp=" << lcp << std::endl;

    designer &io = designer::instance();
    std::cerr << "using " << io.call_sign << std::endl;

}
YACK_UDONE()


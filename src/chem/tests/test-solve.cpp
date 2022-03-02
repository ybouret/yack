

#include "yack/chem/forge.hpp"

#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(solve)
{
    randomized::rand_ ran;

    library          lib;
    components       eq;
    chemical::forge &build = chemical::forge::instance();

    if(argc>1)
    {
        build(eq,lib,argv[1]);
        YACK_CHECK(eq.size());
        YACK_CHECK(eq.is_neutral());
        YACK_CHECK(eq.is_minimal());

        vector<double> C(lib.size(),0);

        lib.fill(C,0.8,ran);

        lib(std::cerr,C);
        eq.display(std::cerr) << std::endl;
        eq.extent(1.0,C);

    }




}
YACK_UDONE()

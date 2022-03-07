

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
        double K = 1e-10;

        if(lib.size()==3)
        {
            // ./build/chem/tests/Debug/test-chem solve '[a++]+2[b-]-[c]'
            K    = 0.01;
            C[1] = 1.51001e-12;
            C[2] = 5.72707e-12;
            C[3] = 1.64167e-10;
        }

        lib(std::cerr << "Cini=",C);
        eq.display(std::cerr) << std::endl;
        const double xi = eq.extent(K,C);
        eq.move(C,xi);
        lib(std::cerr << "Cend=",C);

        const double den = eq.reac.mass_action(K,C);
        if(den>0)
        {
            std::cerr << "Q/K=" << eq.prod.mass_action(1,C)/den << std::endl;
        }
        else
        {
            std::cerr << "blocked" << std::endl;
        }

    }




}
YACK_UDONE()

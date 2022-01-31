#include "yack/chem/equilibrium.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

static inline void test_comb(randomized::bits &ran)
{
    for(size_t nr=0;nr<=4;++nr)
    {
        for(size_t np=0;np<=4;++np)
        {
            if(nr==0&&np==0) continue;
            chemical::library           lib;
            chemical::const_equilibrium eq("eq",1e-3);
            char name[4] = { '[', 'A' , ']', 0 };
            for(size_t i=0;i<nr;++i, ++name[1])
            {
                const species &sp = lib(name);
                const unit_t   nu = ran.in(1,4);
                eq.add(sp,nu);
            }
            for(size_t i=0;i<np;++i, ++name[1])
            {
                const species &sp = lib(name);
                const unit_t   nu = -ran.in(1,4);
                eq.add(sp,nu);
            }
            lib.load("[Na+]:[Cl-]");

            std::cerr << lib << std::endl;
            std::cerr << eq  << std::endl;

            const size_t   nv = lib.size();
            vector<double> C(nv,0);
            vector<double> Ctry(nv,0);
            const double   K0 = eq.K(0);
            eq.solve(K0,C,Ctry);

            for(size_t iter=0;iter<100;++iter)
            {
                lib.fill(C,0.8,ran);
                eq.solve(K0,C,Ctry);
            }



        }
    }
}

YACK_UTEST(eq)
{

    randomized::rand_ ran;

    YACK_SIZEOF(species);
    YACK_SIZEOF(actors);
    YACK_SIZEOF(equilibrium);
    YACK_SIZEOF(const_equilibrium);

    //test_comb(ran);

    chemical::library           lib;
    chemical::const_equilibrium eq("eq",1e-3);
    if(argc>1)
    {
        eq.load(argv[1],lib);
    }

    lib.load("[Na+]:[Cl-]");

    const size_t   nv = lib.size();
    vector<double> C(nv,0);
    vector<double> Ctry(nv,0);

    std::cerr << lib << std::endl;
    std::cerr << eq  << std::endl;

    lib.fill(C,0.8,ran);
    lib(std::cerr,C);


    const double K0 = eq.K(0);
    std::cerr << "mass_action: " << eq.mass_action(K0,C) << std::endl;
    const limits &lm = eq.find_limits(C);
    std::cerr << lm << std::endl;
    eq.drvs_action(Ctry,K0,C);
    std::cerr << "phi=" << Ctry << std::endl;

    return 0;

    if(eq.size())
    {
        eq.solve(K0,C,Ctry);

        for(size_t iter=0;iter<100;++iter)
        {
            lib.fill(C,0.8,ran);
            lib(std::cerr << "Cini=" << std::endl,C);
            eq.solve(K0,C,Ctry);
            lib(std::cerr << "Cend=" << std::endl,C);
            std::cerr << "mass_action: " << eq.mass_action(K0,C) << std::endl;
        }
    }

}
YACK_UDONE()

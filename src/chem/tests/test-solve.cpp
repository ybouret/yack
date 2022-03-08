#include "yack/chem/forge.hpp"
#include "yack/utest/run.hpp"
#include "yack/counting/part.hpp"

using namespace yack;
using namespace chemical;

namespace
{

    static inline
    void add_to(library                &lib,
                components             &eq,
                const readable<size_t> &coef,
                const int               sfac,
                char                   &ch)
    {
        for(size_t i=1;i<=coef.size();++i)
        {
            const int      nu = sfac * int(coef[i]);
            const string   id = ch++;
            const species &sp = lib(id,0);
            eq(sp,nu);
        }
    }

    static inline
    void test_compsolve( randomized::bits &ran)
    {

        size_t    nmax = 3;
        partition reac(nmax);
        partition prod(nmax);

        reac.boot();
        do
        {


            {
                std::cerr << "Reac Only" << std::endl;
                library    lib;
                components eq;
                char       ch = 'a';
                add_to(lib,eq,reac,-1,ch);
                eq.display(std::cerr) << std::endl;
            }

            {
                std::cerr << "Prod Only" << std::endl;
                library    lib;
                components eq;
                char       ch = 'a';
                add_to(lib,eq,reac,1,ch);
                eq.display(std::cerr) << std::endl;
            }

            std::cerr << "Reac/Prod" << std::endl;
            prod.boot();
            std::cerr << "PROD=" << prod << std::endl;
            do
            {
                std::cerr << "\tprod=" << prod << std::endl;
                library    lib;
                components eq;
                char       ch = 'a';
                add_to(lib,eq,reac,-1,ch);
                add_to(lib,eq,prod,+1,ch);
                eq.display(std::cerr) << std::endl;
            }
            while(prod.next());
            
        }
        while(reac.next());


    }

}


YACK_UTEST(solve)
{
    randomized::rand_ ran;

    library          lib;
    components       eq;
    chemical::forge &build = chemical::forge::instance();

    test_compsolve(ran);
    return 0;

    if(argc>1)
    {
        build(eq,lib,argv[1]);
        YACK_CHECK(eq.size());
        YACK_CHECK(eq.is_neutral());
        YACK_CHECK(eq.is_minimal());

        vector<double> C(lib.size(),0);

        lib.fill(C,0.8,ran);
        double K = 1e-8;

        if(eq.nu_p<=0)
        {
            K = 1/K;
        }

        if(eq.topo == "1:1,2")
        {
            // ./build/chem/tests/Debug/test-chem solve '[a++]+2[b-]-[c]'
            K    = 0.01;
            C[1] = 1.51001e-12;
            C[2] = 5.72707e-12;
            C[3] = 1.64167e-10;
        }
        std::cerr << "K=" << K << std::endl;
        vector<double> Cs(lib.size(),0);
        lib(std::cerr << "Cini=",C);
        eq.display(std::cerr) << std::endl;
        const double xi = eq.extent(K,C,Cs);
        std::cerr << "xi=" << xi << std::endl;
        lib(std::cerr << "Cend=",Cs);

        const double den = eq.reac.mass_action(K,Cs);
        if(den>0)
        {
            std::cerr << "Q/K=" << eq.prod.mass_action(1,Cs)/den << std::endl;
        }
        else
        {
            std::cerr << "blocked" << std::endl;
        }

    }




}
YACK_UDONE()

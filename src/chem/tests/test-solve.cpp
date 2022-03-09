#include "yack/chem/forge.hpp"
#include "yack/utest/run.hpp"
#include "yack/counting/part.hpp"
#include "yack/system/env.hpp"

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
    void test_eqsolve(library          &lib,
                      const components &eq,
                      randomized::bits & )
    {
        lib.parse("[Na+]");
        lib.parse("[Cl-]");

        const size_t M = lib.size();
        for(int p=-2;p<=2;++p)
        {
            const double   K = pow(10.0,double(p));
            std::cerr << "---- K=" << K << std::endl;
            vector<double> C0(M,0);
            vector<double> Cs(M,0);
            const double   xi = eq.solve1D(K,C0,Cs);
            lib(std::cerr << "@" << xi << "=",Cs);
        }

    }

    static inline
    void test_compsolve( randomized::bits &ran)
    {

        size_t    nmax = 2;
        partition reac(nmax);
        partition prod(nmax);

        reac.boot();
        do
        {


            {
                library    lib;
                components eq;
                char       ch = 'a';
                add_to(lib,eq,reac,-1,ch);
                eq.display(std::cerr) << std::endl;
                test_eqsolve(lib,eq,ran);
            }

            {
                library    lib;
                components eq;
                char       ch = 'a';
                add_to(lib,eq,reac,1,ch);
                test_eqsolve(lib,eq,ran);
                eq.display(std::cerr) << std::endl;
                test_eqsolve(lib,eq,ran);
            }
            continue;

            prod.boot();
            do
            {
                library    lib;
                components eq;
                char       ch = 'a';
                add_to(lib,eq,reac,-1,ch);
                add_to(lib,eq,prod,+1,ch);
                eq.display(std::cerr) << std::endl;
                test_eqsolve(lib,eq,ran);
            }
            while(prod.next());
            
        }
        while(reac.next());


    }

}


YACK_UTEST(solve)
{
    chemical::entity::verbose =environment::flag("VERBOSE");
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
        const double xi = eq.solve1D(K,C,Cs);
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
    else
    {
        test_compsolve(ran);
    }



}
YACK_UDONE()

#include "yack/chem/forge.hpp"
#include "yack/utest/run.hpp"
#include "yack/counting/part.hpp"
#include "yack/counting/comb.hpp"
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
                      randomized::bits &ran)
    {
        static const char fn[] = "solve.dat";
        lib.parse("[Na+]");
        lib.parse("[Cl-]");

        const size_t M = lib.size();
        ios::ocstream::overwrite(fn);
        unsigned count = 0;

        for(int p=-12;p<=12;++p)
        {
            const double   K = pow(10.0,double(p));
            std::cerr << "---- K=" << K << std::endl;
            vector<double> C0(M,0);
            vector<double> Cs(M,0);
            {
                const double   xi  = eq.solve1D(K,C0,Cs);
                const double   rho = eq.Q(Cs)/K;
                //std::cerr << "0 -> (" << xi << ",@" << rho << ") " << Cs << std::endl;
                if(rho>0)
                    ios::ocstream::echo(fn,"%u %g %g\n",++count,rho,xi);
            }


            const size_t n = eq.size();
            for(size_t k=1;k<=n;++k)
            {
                combination comb(n,k);
                do
                {
                    std::cerr << "\t-- using ";
                    for(size_t i=1;i<=comb.size();++i)
                    {
                        const component &c = eq[ comb[i] ];
                        std::cerr << " [" << (*c).name << "]";
                    }
                    std::cerr << " of ";
                    eq.display(std::cerr) << " @K=" << K << std::endl;

                    for(size_t iter=0;iter<64;++iter)
                    {
                        C0.ld(0);
                        for(size_t i=comb.size();i>0;--i)
                        {
                            C0[ comb[i] ] = species::concentration(ran);
                        }
                        //std::cerr << "\tC0=" << C0;
                        const double xi  = eq.solve1D(K,C0,Cs);
                        const double rho = eq.Q(Cs)/K;
                        //std::cerr << "-> (" << xi << ",@" << rho << ") " << Cs << std::endl;
                        if(rho>0)
                        {
                            ios::ocstream::echo(fn,"%u %g %g #%s\n",++count,rho,xi, eq.topo() );
                        }
                    }

                } while(comb.next());
            }

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
                std::cerr << std::endl;
                library    lib;
                components eq;
                char       ch = 'a';
                add_to(lib,eq,reac,-1,ch);
                eq.display(std::cerr) << std::endl;
                test_eqsolve(lib,eq,ran);
            }

            {
                std::cerr << std::endl;
                library    lib;
                components eq;
                char       ch = 'a';
                add_to(lib,eq,reac,1,ch);
                eq.display(std::cerr) << std::endl;
                test_eqsolve(lib,eq,ran);
            }
            
            prod.boot();
            do
            {
                std::cerr << std::endl;
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

        std::cerr << "Q/K=" << eq.Q(Cs)/K << std::endl;

        vector<double> psi(lib.size(),0);
        vector<double> tmp(lib.size(),0);
        eq.drvs_action(psi,K,Cs,tmp);

        std::cerr << "psi=" << psi << std::endl;

    }
    else
    {
        test_compsolve(ran);
    }



}
YACK_UDONE()

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
                      randomized::bits &ran,
                      size_t           &calls)
    {
        static const char fn[] = "brew.dat";
        lib.parse("[Na+]");
        lib.parse("[Cl-]");

        const size_t M = lib.size();
        ios::ocstream::overwrite(fn);
        unsigned count = 0;

        for(int p=-14;p<=14;++p)
        {
            const double   K = pow(10.0,double(p));
            std::cerr << "---- K=" << K << std::endl;
            vector<double> C0(M,0);
            vector<double> Cs(M,0);

            // test zero
            {
                const outcome  res = eq.brew1D(K,C0,Cs,&calls);
                const double   rho = eq.Q(Cs)/K;
                //std::cerr << "0 -> (" << xi << ",@" << rho << ") " << Cs << std::endl;
                if(rho>0)
                {
                    ios::ocstream::echo(fn,"%u %g %g\n",++count,rho,res.xi0);
                }
                else
                {
                    YACK_ASSERT(fabs(rho)<=0);
                }
            }

            if(true)
            {
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
                            const outcome  res = eq.brew1D(K,C0,Cs,&calls);
                            //std::cerr << "xi=" << res.xi0 << "; Cs=" << Cs << std::endl;

                            const double rho = eq.Q(Cs)/K;
                            //std::cerr << "-> (" << xi << ",@" << rho << ") " << Cs << std::endl;
                            if(rho>0)
                            {
                                ios::ocstream::echo(fn,"%u %g %g #%s\n",++count,rho,res.xi0, eq.topo() );
                            }
                            else
                            {
                                YACK_ASSERT(fabs(rho)<=0);
                            }
                        }

                    } while(comb.next());
                }
            }

        }

    }

    static inline
    void test_compsolve( randomized::bits &ran)
    {

        size_t    nmax = 4;
        partition reac(nmax);
        partition prod(nmax);
        size_t    calls = 0;
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
                test_eqsolve(lib,eq,ran,calls);
            }

            {
                std::cerr << std::endl;
                library    lib;
                components eq;
                char       ch = 'a';
                add_to(lib,eq,reac,1,ch);
                eq.display(std::cerr) << std::endl;
                test_eqsolve(lib,eq,ran,calls);
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
                test_eqsolve(lib,eq,ran,calls);
            }
            while(prod.next());

        }
        while(reac.next());

        std::cerr << std::endl;
        std::cerr << "#call=" << calls << std::endl;
    }

}

YACK_UTEST(brew)
{
    chemical::entity::verbose = environment::flag("VERBOSE");
    randomized::rand_ ran;

    test_compsolve(ran);

}
YACK_UDONE()
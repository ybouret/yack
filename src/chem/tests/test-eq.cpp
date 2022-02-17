#include "yack/chem/equilibrium.hpp"
#include "yack/chem/library.hpp"
#include "yack/counting/part.hpp"
#include "yack/arith/ipower.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;


static inline void test_comb(randomized::bits &ran)
{

    size_t count =0;
    for(size_t r=1;r<=4;++r)
    {
        partition rp(r);
        rp.boot();
        do
        {
            const size_t  nr = rp.size();
            for(size_t p=1;p<=4;++p)
            {
                partition pp(p);
                pp.boot();
                do
                {
                    const size_t    np=pp.size();
                    for(int q=0;q<=12;++q)
                    {
                        chemical::library           lib;
                        chemical::const_equilibrium eq("eq",ipower<double,int>(0.1,q));
                        char name[4] = { '[', 'A' , ']', 0 };
                        for(size_t i=1;i<=nr;++i, ++name[1])
                        {
                            const species &sp = lib(name);
                            const unit_t   nu = -unit_t(rp[i]);
                            eq.add(sp,nu);
                        }
                        for(size_t i=1;i<=np;++i, ++name[1])
                        {
                            const species &sp = lib(name);
                            const unit_t   nu = unit_t(pp[i]);
                            eq.add(sp,nu);
                        }
                        lib.load("[Na+]:[Cl-]");
                        if(0==q)
                        {
                            //std::cerr << lib << std::endl;
                            std::cerr << rp << "->" << pp << " | ";
                            std::cerr << eq;
                            std::cerr << std::endl;
                        }

                        ++count;

                        const size_t   nv = lib.size();
                        vector<double> C(nv,0);
                        vector<double> Ctry(nv,0);
                        const double   K0 = eq.K(0);

                        // solve zero
                        eq.solve(K0,C,Ctry);

                        // solve other
                        for(size_t iter=1;iter<=256;++iter)
                        {
                            lib.fill(C,0.8,ran);
                            eq.solve(K0,C,Ctry);
                            std::cerr << ' ' << std::setw(14) << eq.mass_action(K0,C);
                            if( 0 == (iter%4) ) std::cerr << std::endl;
                        }

                    }
                } while(pp.next());

            }


        } while(rp.next());

    }
    std::cerr << "Tested " << count << std::endl;


}

YACK_UTEST(eq)
{

    randomized::rand_ ran;



    if(argc>1)
    {
        chemical::library           lib;
        chemical::const_equilibrium eq("eq",1e-3);
        eq.load(argv[1],lib);
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
        const limits &lm = eq.find_private_limits(C);
        std::cerr << lm << std::endl;
        eq.drvs_action(Ctry,K0,C);
        std::cerr << "phi=" << Ctry << std::endl;

        if(eq.size())
        {
            const species *vanishing = NULL;
            const double xi = eq.extent(K0,C,Ctry,vanishing);
            std::cerr << "xi=" << xi << " / ";
            if(vanishing) std::cerr << *vanishing; else std::cerr << " no vanishing";
            std::cerr << std::endl;

            eq.solve(K0,C,Ctry);
            lib(std::cerr,C);
        }

    }
    else
    {
        test_comb(ran);
    }

    YACK_SIZEOF(species);
    YACK_SIZEOF(actor);
    YACK_SIZEOF(actors);
    YACK_SIZEOF(equilibrium);
    YACK_SIZEOF(const_equilibrium);

    


}
YACK_UDONE()

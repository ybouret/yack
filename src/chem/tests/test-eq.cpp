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
                        std::cerr << eq.mass_action(K0,C);
                        // solve other
                        for(size_t iter=0;iter<128;++iter)
                        {
                            lib.fill(C,0.8,ran);
                            eq.solve(K0,C,Ctry);
                            std::cerr << '/' << eq.mass_action(K0,C);
                        }
                        std::cerr << std::endl;

                    }
                } while(pp.next());

            }


        } while(rp.next());


        std::cerr << "Tested " << count << std::endl;

    }

    return;

    for(size_t nr=0;nr<=4;++nr)
    {
        for(size_t np=0;np<=4;++np)
        {
            if(nr==0&&np==0) continue;
            for(size_t config=0;config<4;++config)
            {
                chemical::library           lib;
                chemical::const_equilibrium eq("eq",species::concentration(ran));
                char name[4] = { '[', 'A' , ']', 0 };
                for(size_t i=0;i<nr;++i, ++name[1])
                {
                    const species &sp = lib(name);
                    const unit_t   nu = -ran.in(1,4);
                    eq.add(sp,nu);
                }
                for(size_t i=0;i<np;++i, ++name[1])
                {
                    const species &sp = lib(name);
                    const unit_t   nu = ran.in(1,4);
                    eq.add(sp,nu);
                }
                lib.load("[Na+]:[Cl-]");

                std::cerr << lib << std::endl;
                std::cerr << eq  << std::endl;

                const size_t   nv = lib.size();
                vector<double> C(nv,0);
                vector<double> Ctry(nv,0);
                const double   K0 = eq.K(0);

                // solve 0
                eq.solve(K0,C,Ctry);

                // solve other
                for(size_t iter=0;iter<1000;++iter)
                {
                    lib.fill(C,0.8,ran);
                    try {
                        eq.solve(K0,C,Ctry);
                        //lib(std::cerr,C);
                    }
                    catch(...)
                    {
                        lib(std::cerr,C);
                        std::cerr << eq << std::endl;
                        throw;
                    }
                }
            }
        }
    }
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
        const limits &lm = eq.find_limits(C);
        std::cerr << lm << std::endl;
        eq.drvs_action(Ctry,K0,C);
        std::cerr << "phi=" << Ctry << std::endl;

        if(eq.size())
        {
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

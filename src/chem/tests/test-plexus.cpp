#include "yack/chem/lua/equilibria.hpp"
#include "yack/chem/plexus.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"
#include "yack/counting/comb.hpp"

using namespace yack;
using namespace chemical;


YACK_UTEST(plexus)
{
    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_ ran;
    library           lib;
    lua_equilibria    eqs;

    eqs->dostring("function F(t) return 2+math.sin(t); end");

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }



    plexus cs(lib,eqs);
    std::cerr << "lib=" << cs.lib << std::endl;
    std::cerr << "sub=" << cs.sub << std::endl;
    std::cerr << "eqs=" << cs.eqs << std::endl;

    vector<species*> active(cs.active.size,as_capacity);
    for(const anode *node=cs.active.head;node;node=node->next)
    {
        active << (species *) & **node;
    }
    const size_t na = active.size();

    for(size_t i=1;i<=na;++i)
    {
        //std::cerr << "  (*) " << active[i]->name << std::endl;
    }

    vector<double> C(cs.M,0);

    cs.computeK(0);

    C.ld(0);
    for(size_t iter=0;iter<100;++iter)
    {
        lib.fill(C, 1, ran);
        if(cs.solve(C))
        {
            lib(std::cerr<<"Cend=",C);
        }
    }

    return 0;

    for(size_t nz=1;nz<=active.size();++nz)
    {
        combination comb(na,nz);
        do
        {
            std::cerr << "-- using";
            for(size_t i=1;i<=nz;++i)
            {
                std::cerr << " " << active[comb[i]]->name;
            }
            std::cerr << std::endl;

            {
                C.ld(0);
                for(size_t i=nz;i>0;--i)
                {
                    comb(C,i) = 0;
                    C[ comb[i] ] = species::concentration(ran);
                }
            }
            lib(std::cerr << "Cini=",C);
            cs.solve(C);
            lib(std::cerr << "Cend=",C);

        } while(comb.next());

    }


    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;
    
    // VERBOSE=1 ./build/chem/tests/Debug/test-chem plexus 'ab:-[a]+[b]:1' 'ac:-[a]+[c]:2' 'bc:-[b]+2[c]:3'
}
YACK_UDONE()



#include "yack/chem/reactor.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"
#include "yack/counting/comb.hpp"


using namespace yack;
using namespace chemical;

YACK_UTEST(reactor)
{
    entity::verbose = environment::flag("VERBOSE");
    size_t            iter = 5;
    randomized::rand_ ran;
    library           lib;
    lua_equilibria    eqs;

    eqs->dostring("function F(t) return 2+math.sin(t); end");

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }


    reactor        cs(lib,eqs,0.0);
    vector<double> C(cs.M,0);

    C.ld(0);

    if(!cs.solve(C))
    {
        throw exception("cannot solve null phase space!!");
    }
    lib(std::cerr<<"Cend=",C);
    cs.display(C);

    std::cerr << "Active:";
    vector<species *> active(cs.active.size,as_capacity);
    for(const anode *node=cs.active.head;node;node=node->next)
    {
        active << & coerce(**node);
        std::cerr << " " << (active.back()->name);
    }
    std::cerr << std::endl;


    const size_t n = active.size();


    for(size_t k=1;k<=n;++k)
        //for(size_t k=n;k>0;--k)
    {
        combination comb(n,k);
        do {
            vector<species *> sub(k,as_capacity);
            comb.extract(sub,active);

            std::cerr << "---- using";
            for(size_t i=1;i<=k;++i)
            {
                std::cerr << " " << sub[i]->name;
            }
            std::cerr << std::endl;

            std::cerr << "[";
            for(size_t it=0;it<iter;++it)
            {
                C.ld(0);
                for(size_t i=k;i>0;--i)
                {
                    const size_t j = **sub[i];
                    C[j] = species::concentration(ran);
                }
                (std::cerr << ".").flush();
                //lib(std::cerr<<"Cini=",C);
                if(!cs.solve(C))
                {
                    lib(std::cerr<<"Cini=",C);
                    entity::verbose = true;
                    (void)cs.solve(C);
                    //cs.display(C);
                    throw exception("Couldn't solve!");
                }
            }
            std::cerr << "]" << std::endl;
            std::cerr << std::endl;
        }
        while(comb.next());
    }




    return 0;

    for(size_t iter=0;iter<100;++iter)
    {
        lib.fill(C, 1, ran);
        if( cs.steady(C))
        {
            lib(std::cerr<<"Cend=",C);
            cs.display(C);
        }
    }


}
YACK_UDONE()



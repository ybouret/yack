#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/reactor.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"
#include "yack/counting/comb.hpp"
#include "yack/math/opt/locate.hpp"
#include "yack/math/opt/optimize.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(reactor)
{
    entity::verbose         = environment::flag("VERBOSE");
    math::locate::verbose   = environment::flag("LOCATE");
    math::optimize::verbose = environment::flag("OPTIMIZE");

    randomized::rand_ ran;
    library           lib;
    luaEquilibria     eqs;


    eqs->dostring("function f(t) return 1.1+math.cos(t) end");

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "Testing Reactor with" << std::endl;
    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;
    std::cerr << "\t ----- ready to compute ----" << std::endl;

    reactor        cs(lib,eqs,0.0);
    vector<double> C(cs.M,0);


    std::cerr << "[Solving Zero State]" << std::endl;
    if( cs.solve(C) )
    {
        lib(std::cerr << "Cend=", "", C);
    }
    else
    {
        throw exception("couldn't solve zero phase state!!");
    }

    std::cerr << "[Solving Random State]" << std::endl;
    lib.fill(C,ran);
    if( cs.solve(C) )
    {
        lib(std::cerr << "Cend=", "", C);
    }
    else
    {
        throw exception("couldn't solve random phase state!!");
    }

    

    vector<species*> psp;
    for(const anode *node=cs.working.head;node;node=node->next)
    {
        psp << & coerce(**node);
    }

    const size_t n = psp.size();
    if(n)
    {
        for(size_t nz=1;nz<=n;++nz)
        {
            vector<species *> sub(nz);
            combination       comb(n,nz);

            do
            {
                comb.extract(sub,psp);
                std::cerr << "-------- using";
                for(size_t i=1;i<=nz;++i)
                {
                    std::cerr << ' ' << '[' << sub[i]->name << ']';
                }
                std::cerr << " --------" << std::endl;

                {
                    C.ld(0);
                    for(size_t i=1;i<=nz;++i)
                    {
                        C[ **sub[i] ] = library::concentration(ran);
                    }
                    //lib(std::cerr << "Cini=", "", C);
                    if(!cs.solve(C))
                    {
                        std::cerr << "Couldn't solve " << C << std::endl;
                        exit(1);
                    }
                }

            }
            while(comb.next());

        }
    }
    else
    {
        std::cerr << "no active species..." << std::endl;
    }



}
YACK_UDONE()

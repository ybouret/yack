

#include "yack/chem/mix/groups.hpp"
#include "yack/chem/mix/couples.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/chem/library.hpp"
#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/cxx-array.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(reactor)
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

    equilibria couples;
    equilibria lattice;

    std::cerr << lib << std::endl;
    std::cerr << "singles=" << eqs << std::endl;

    if(eqs.size() && lib.size())
    {
        library        sub(lib);
        matrix<int>    Nu(eqs.size(),lib.size());
        vector<double> K(eqs.size(),0);
        eqs.build(Nu,K,0.0);


        coupling::build(couples,eqs,Nu,K,lib);
        std::cerr << "couples=" << couples << std::endl;

        lattice << eqs << couples;
        std::cerr << "lattice=" << lattice << std::endl;

        groups top(lattice);
        std::cerr << top << std::endl;
        matrix<bool> detached;
        lattice.build(detached);
        
        lattice(std::cerr << "detached=",detached,"");
        
        const size_t      dims = top.size;
        cxx_array<groups> part(dims);
        
        {
            size_t i=0;
            for(const group *attached=top.head;attached;attached=attached->next)
            {
                groups G(*attached,detached);
                part[++i].swap_with(G);
            }
        }

        std::cerr << "part=" << part << std::endl;
        groups grp;
        grp.weave(part);

    }
   



}
YACK_UDONE()



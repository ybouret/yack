

#include "yack/chem/mix/groups.hpp"
#include "yack/chem/mix/couples.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/chem/library.hpp"
#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"
#include "yack/chem/mix/independency.hpp"
#include "yack/ptr/auto.hpp"


#include "yack/chem/reactor.hpp"

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

    if(false)
    {
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

            if(!independency::of(Nu))
            {
                throw exception("singular equilibria");
            }



            coupling::build(couples,eqs,Nu,K,sub);
            std::cerr << "couples=" << couples << std::endl;

            lattice << eqs << couples;
            std::cerr << "lattice=" << lattice << std::endl;


            auto_ptr<groups> cls = groups::create_from(lattice);
            std::cerr << cls << std::endl;

        }
    }


    reactor cs(lib,eqs,0.0);



}
YACK_UDONE()



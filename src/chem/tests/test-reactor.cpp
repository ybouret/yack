#include "yack/chem/reactor.hpp"
#include "yack/chem/eqs/lua.hpp"
#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;



YACK_UTEST(reactor)
{
    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_  ran;
    library            lib;
    luaEquilibria      eqs;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "[[ lib ]]" << std::endl;
    std::cerr << lib << std::endl;
    std::cerr << std::endl;

    std::cerr << "[[ eqs ]]" << std::endl;
    std::cerr << eqs << std::endl;
    std::cerr << std::endl;

    reactor cs(lib,eqs,0.0);

    YACK_SIZEOF(entity);
    YACK_SIZEOF(alist);
    YACK_SIZEOF(glist);
    YACK_SIZEOF(cluster);
    YACK_SIZEOF(clusters);
    YACK_SIZEOF(reactor);
    YACK_SIZEOF(sp_repo);
    YACK_SIZEOF(eq_group);
    YACK_SIZEOF(sp_group);
    YACK_SIZEOF(eq_tier);
    YACK_SIZEOF(sp_tier);
    YACK_SIZEOF(umap);
    YACK_SIZEOF(udict);
    


    {

        ios::ocstream fp("reactor.dot");
        ios::vizible::digraph_init(fp,"G");
        for(const cluster *cl=cs.linked->head;cl;cl=cl->next)
        {
            cl->viz(fp);
        }

        ios::vizible::digraph_quit(fp);
    }

    ios::vizible::render("reactor.dot");

}
YACK_UDONE()

#include "yack/concurrent/topology.hpp"
#include "yack/utest/run.hpp"
#include "yack/ptr/auto.hpp"

using namespace yack;

YACK_UTEST(sync_topology)
{
    YACK_SIZEOF(concurrent::quark::topology);

    auto_ptr<concurrent::quark::topology> topo;

    if(argc>1)
    {
        topo = new concurrent::quark::topology(argv[1]);
    }
    else
    {
        topo = new concurrent::quark::topology();
    }

    std::cerr << topo << std::endl;

}
YACK_UDONE()



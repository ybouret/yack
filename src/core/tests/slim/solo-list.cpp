#include "yack/data/slim/solo-list.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"
#include "yack/data/list/ops.hpp"


using namespace  yack;

YACK_UTEST(slim_solo_list)
{
    randomized::rand_ ran;

    solo_list<apn>       ml;
    solo_list<const apn> cl;
    YACK_SIZEOF(solo_list<apn>::node_type);

    for(size_t i=5+ran.leq(5);i>0;--i)
    {
        const apn n(ran,1+ran.leq(16));
        const apn m(ran,1+ran.leq(16));
        ml << n; ml >> m;
        cl << n; cl >> m;
    }

    std::cerr << ml << std::endl;
    std::cerr << cl << std::endl;
    YACK_CHECK( list_ops::are_equal(ml,cl) );

    ml.sort_with(comparison::increasing<apn>);
    cl.sort_with(comparison::decreasing<apn>);
    std::cerr << ml << std::endl;
    std::cerr << cl << std::endl;

}
YACK_UDONE()


#include "yack/data/slim/solo-repo.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/data/list/ops.hpp"

using namespace  yack;

YACK_UTEST(slim_solo_repo)
{
    randomized::rand_ ran;

    vector<apq>       db;
    for(size_t i=5+ran.leq(5);i>0;--i)
    {
        const apq q(ran,10);
        db << q;
    }

    std::cerr << "db=" << db << std::endl;

    solo_repo<apq>       mr;
    solo_repo<const apq> cr;
    for(size_t i=db.size();i>0;--i)
    {
        mr << db[i]; mr >> db[i];
        cr << db[i]; cr >> db[i];
    }

    std::cerr << "mr=" << mr << std::endl;
    std::cerr << "cr=" << cr << std::endl;

    {
        const solo_repo<apq> mr2 = mr;
        YACK_CHECK( list_ops::are_equal(mr2,mr) );
    }


    mr.sort_with(comparison::increasing<apq>);
    cr.sort_with(comparison::decreasing<apq>);
    
    std::cerr << "mr=" << mr << std::endl;
    std::cerr << "cr=" << cr << std::endl;

    YACK_SIZEOF(solo_repo<apq>::node_type);
}
YACK_UDONE()


#include "yack/data/slim/coop-repo.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include "yack/sequence/vector.hpp"


using namespace  yack;

YACK_UTEST(slim_coop_repo)
{
    randomized::rand_ ran;

    vector<apq>       db;
    for(size_t i=5+ran.leq(5);i>0;--i)
    {
        const apq q(ran,10);
        db << q;
    }

    coop_repo<apq>::fund_type       mr_fund = new coop_repo<apq>::bank_type();
    coop_repo<const apq>::fund_type cr_fund = new coop_repo<const apq>::bank_type();

    coop_repo<apq>       mr(mr_fund);
    coop_repo<const apq> cr(cr_fund);

    for(size_t i=db.size();i>0;--i)
    {
        mr << db[i]; mr >> db[i];
        cr << db[i]; cr >> db[i];
    }

    std::cerr << "mr=" << mr << std::endl;
    std::cerr << "cr=" << cr << std::endl;

    mr.sort_with(comparison::increasing<apq>);
    cr.sort_with(comparison::decreasing<apq>);

    std::cerr << "mr=" << mr << std::endl;
    std::cerr << "cr=" << cr << std::endl;

    coop_repo<apq>::warden_type keep(mr.pop_front(),mr);
    std::cerr << "front was " << keep << std::endl;



    YACK_SIZEOF(coop_repo<apq>::node_type);

}
YACK_UDONE()


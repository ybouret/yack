
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
}
YACK_UDONE()


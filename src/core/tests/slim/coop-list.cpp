
#include "yack/data/slim/coop-list.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"



using namespace  yack;

YACK_UTEST(slim_coop_list)
{
    randomized::rand_ ran;

    typedef coop_list<apn>       ml_type;
    typedef coop_list<const apn> cl_type;

    ml_type::fund_type ml_fund = new ml_type::bank_type();
    cl_type::fund_type cl_fund = new cl_type::bank_type();

    {
        coop_list<apn>       ml(ml_fund);
        coop_list<const apn> cl(cl_fund);

        YACK_SIZEOF(coop_list<apn>::node_type);

        for(size_t i=5+ran.leq(5);i>0;--i)
        {
            const apn n(ran,1+ran.leq(16));
            //std::cerr << "n=" << n << std::endl;
            const apn m(ran,1+ran.leq(16));
            ml << n; ml >> m;
            cl << n; cl >> m;
        }

        std::cerr << ml << std::endl;
        std::cerr << cl << std::endl;

        ml.sort_with(comparison::increasing<apn>);
        cl.sort_with(comparison::decreasing<apn>);
        std::cerr << ml << std::endl;
        std::cerr << cl << std::endl;
    }

    std::cerr << "#fund: " << ml_fund->size << " / " << cl_fund->size << std::endl;

}
YACK_UDONE()

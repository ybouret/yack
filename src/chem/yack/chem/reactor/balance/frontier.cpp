
#include "yack/chem/reactor/balance/frontier.hpp"

namespace yack {

    namespace chemical
    {

        frontier:: ~frontier() throw()
        {
        }

        frontier:: frontier(const sp_fund &fund) throw() : sp_para(fund), xi(0) {}
        

    }

}

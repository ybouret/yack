#include "yack/data/slim/solo-list.hpp"
#include "yack/data/slim/coop-list.hpp"
#include "yack/data/slim/list.hpp"
#include "yack/data/list/ops.hpp"

#include "yack/utest/run.hpp"

using namespace yack;
template <typename LIST>
void process(LIST &L)
{
    typedef typename LIST::node_type node_t;
    L.erase();
    
    node_t *p = 0;
    int     i = 1;
    YACK_CHECK(!list_ops::search(L,i,p));
    
    
}

YACK_UTEST(slim_ops)
{
    coop_list<int>::fund_type fund = new coop_list<int>::bank_type();
    slim_list<int>            dataList;
    solo_list<int>            soloList;
    coop_list<int>            coopList(fund);
    
    typedef slim_node<int> node_t;
    
    process(dataList);
    process(soloList);
    process(coopList);
    
    
    
    
    
}
YACK_UDONE()


#include "yack/data/slim/solo-list.hpp"
#include "yack/data/slim/coop-list.hpp"
#include "yack/data/slim/list.hpp"
#include "yack/data/list/ops.hpp"

#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(slim_ops)
{
    coop_list<int>::fund_type fund = new coop_list<int>::bank_type();
    slim_list<int> dataList;
    solo_list<int> soloList;
    coop_list<int> coopList(fund);
    
    typedef slim_node<int> node_t;
    
    node_t *p=0;
    int     i=1;
    if( list_ops::search(dataList,i,p) )
    {
        
    }
    else
    {
        if(p)
        {
            dataList.insert_after(p,new node_t(i,transmogrify));
        }
        else
        {
            dataList >> i;
        }
    }
    
    
}
YACK_UDONE()


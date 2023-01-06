#include "yack/data/slim/solo-list.hpp"
#include "yack/data/slim/coop-list.hpp"
#include "yack/data/slim/list.hpp"
#include "yack/data/list/ops.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/utest/run.hpp"
#include "yack/randomized/shuffle.hpp"

using namespace yack;
template <typename LIST>
void process(LIST &L, randomized::bits &ran)
{
    typedef typename LIST::node_type node_t;

    node_t     *p = 0;
    vector<int> k;
    for(size_t iter=10;iter>0;--iter)
    {
        L.erase();
        k.free();
        const size_t max_size = 4 + ran.leq(10);
        while( L.size < max_size)
        {
            const int i = static_cast<int>( ran.in(-100,100) );
            if( list_ops::search(L, i, p) ) continue;
            if(p)
            {
                L.after(p,i);
            }
            else
            {
                L.shove(i);
            }
            YACK_ASSERT(list_ops::is_increasing(L));
            k << i;
        }
        std::cerr << L << std::endl;

        randomized::shuffle::tableau(k,ran);

        while(k.size())
        {
            const int i = k.back();
            k.pop_back();
            YACK_ASSERT(list_ops::search(L,i,p));
            L.pluck(p);
        }

    }



}

YACK_UTEST(slim_ops)
{
    randomized::rand_ ran;

    coop_list<int>::fund_type fund = new coop_list<int>::bank_type();
    slim_list<int>            dataList;
    solo_list<int>            soloList;
    coop_list<int>            coopList(fund);
    

    process(dataList,ran); std::cerr << std::endl;
    process(soloList,ran); std::cerr << std::endl;
    process(coopList,ran);
    std::cerr << "#fund=" << fund->size << std::endl;
    std::cerr << "#solo=" << soloList.cache->size << std::endl;
    
    
    
}
YACK_UDONE()


#include "yack/data/dinky/solo-list.hpp"
#include "yack/data/dinky/coop-list.hpp"
#include "yack/data/dinky/core-list.hpp"
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
        L.clear();
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

        std::cerr << "\thead=" << L.pop_head() << std::endl;
        std::cerr << "\ttail=" << L.pop_tail() << std::endl;


        randomized::shuffle::tableau(k,ran);

        while(k.size())
        {
            const int i = k.back();
            k.pop_back();
            if(list_ops::search(L,i,p))
                L.cut(p);
        }

        for(int i=0;i<10;++i) L << i;
        std::cerr << "L=" << L << std::endl;
    }

    L.clear();


}

YACK_UTEST(dinky_ops)
{
    randomized::rand_ ran;

    dinky_coop_list<int>::proxy_type fund = new dinky_coop_list<int>::zpool_type();
    dinky_core_list<int>            dataList;
    dinky_solo_list<int>            soloList;
    dinky_coop_list<int>            coopList(fund);
    

    process(dataList,ran); std::cerr << std::endl; 
    process(soloList,ran); std::cerr << std::endl;
    process(coopList,ran);
    std::cerr << "#fund=" << fund->stowage()  << std::endl;
    std::cerr << "#solo=" << soloList.ready() << std::endl;
    
    
    
}
YACK_UDONE()


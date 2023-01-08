

#include "yack/data/dinky/manifest.hpp"
#include "yack/data/dinky/znone.hpp"
#include "yack/data/dinky/zsolo.hpp"
#include "yack/data/dinky/zcoop.hpp"
#include "yack/data/dinky/zproxy.hpp"

#include "yack/utest/run.hpp"

#include "yack/sequence/vector.hpp"

using namespace yack;

namespace
{
    
    
}

YACK_UTEST(dinky_manifest)
{
    randomized::rand_ ran;
    typedef dinky_node< dinky_ptr<const int> > node_t;
    zcoop<node_t>::proxy fund = new zcoop<node_t>();
    
    
    vector<int> arr;
    
    for(size_t i=2+ran.leq(10);i>0;--i)
    {
        arr << static_cast<int>( ran.in(-100,100) );
    }
    
    dinky_manifest<int,znone> rn;
    dinky_manifest<int,zsolo> rs;
    dinky_manifest<const int,zcoop> rc(fund);
    
    for(size_t i=arr.size();i>0;--i)
    {
        rn << arr[i]; rn >> arr[i];
        rs << arr[i]; rs >> arr[i];
        rc << arr[i]; rc >> arr[i];
    }
    
    std::cerr << rn << std::endl;
    std::cerr << rs << std::endl;
    std::cerr << rc << std::endl;

    
    
}
YACK_UDONE()

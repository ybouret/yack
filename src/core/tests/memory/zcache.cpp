
#include "yack/memory/small/zcache.hpp"
#include "yack/ios/characters.hpp"
#include "yack/data/small/node.hpp"
#include "yack/utest/run.hpp"

using namespace yack;


YACK_UTEST(memory_zcache)
{
    randomized::rand_ ran;
    
    {
        memory::zcache zc(22);
        
        zc.load(100);
        void        *addr[2000];
        const size_t size = sizeof(addr)/sizeof(addr[0]);
        for(size_t i=0;i<size;++i)
        {
            addr[i] = zc.acquire_unlocked();
        }
        
        randomized::shuffle::data(addr,size,ran);
        for(size_t i=0;i<size;++i)
        {
            zc.release_unlocked(addr[i]);
        }
    }
    
    ios::characters ch;
    for(size_t i=0;i<=10;++i)
    {
        ch << uint8_t('a' + i);
    }

    ch.display(std::cerr) << std::endl;
    std::cerr << ch << std::endl;

    YACK_SIZEOF(ios::character);
    YACK_SIZEOF(small_node<uint8_t>);
    
    
}
YACK_UDONE()


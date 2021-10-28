
#include "yack/memory/small/zcache.hpp"
#include "yack/utest/run.hpp"
#include <cstring>
#include <cerrno>

using namespace yack;

namespace
{
    
}

YACK_UTEST(memory_zcache)
{
    randomized::rand_ ran;
    
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
YACK_UDONE()


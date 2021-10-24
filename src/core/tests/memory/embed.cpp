#include "yack/memory/embed.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/global.hpp"

using namespace yack;

YACK_UTEST(memory_embed)
{
    char  *a = NULL;
    int   *b = NULL;
    float *c = NULL;
    
    memory::embed emb[] =
    {
        memory::embed(a,2),
        memory::embed(b,7),
        memory::embed(c,11)
    };
    
    const size_t num = sizeof(emb)/sizeof(emb[0]);
    for(size_t i=0;i<num;++i)
    {
        std::cerr << emb[i] << std::endl;
    }
    size_t bs   = 0;
    void  *wksp = YACK_MEMORY_EMBED(emb,memory::global::instance(),bs);
    
    for(size_t i=0;i<num;++i)
    {
        std::cerr << emb[i] << std::endl;
    }
    memory::global::location().release(wksp,bs);
    
    
}
YACK_UDONE()


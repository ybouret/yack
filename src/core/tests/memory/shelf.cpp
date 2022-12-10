#include "yack/memory/shelf.hpp"
#include "yack/memory/embed.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/pooled.hpp"

using namespace yack;

YACK_UTEST(memory_shelf)
{
    memory::shelf lib;
    memory::allocator &da = memory::dyadic::instance();
    memory::allocator &ga = memory::global::instance();
    memory::allocator &pa = memory::pooled::instance();

    {
        size_t bytes = 100;
        void  *entry = da.acquire(bytes, 2);
        lib.store(entry,bytes,da);
    }
    
    lib.empty();
    lib.store(0,0,da); lib.empty();
    lib.store(0,0,ga); lib.empty();
    lib.store(0,0,pa); lib.empty();
    
    {
        size_t *indx = 0;
        memory::embed emb[] =
        {
            memory::embed(indx,19)
        };
        lib.build(emb,sizeof(emb)/sizeof(emb[0]),da);
        std::cerr << "indx@" << indx << std::endl;
       
        lib.build(emb,sizeof(emb)/sizeof(emb[0]),ga);
        std::cerr << "indx@" << indx << std::endl;

        lib.build(emb,sizeof(emb)/sizeof(emb[0]),pa);
        std::cerr << "indx@" << indx << std::endl;
    }
    
   

    
    
    
}
YACK_UDONE()

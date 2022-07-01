
#include "yack/concurrent/queue/pipeline.hpp"
#include "yack/memory/allocator/dyadic.hpp"


namespace yack
{
    namespace concurrent
    {
        pipeline:: drone:: drone(pipeline    &boss,
                                 const size_t size,
                                 const size_t rank) :
        next(0),
        prev(0),
        cond(),
        task(NULL),
        ctx(size,rank),
        thr(pipeline::entry,&boss)
        {
        }

        pipeline:: drone:: ~drone() throw()
        {
        }


        pipeline::drone * pipeline::drone:: zalloc( size_t &capa )
        {
            static memory::dyadic &mgr = memory::dyadic::instance();
            return static_cast<drone*>(mgr.acquire(capa,sizeof(drone)))-1;
        }
    }
}

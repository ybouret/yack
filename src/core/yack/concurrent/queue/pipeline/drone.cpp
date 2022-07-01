
#include "yack/concurrent/queue/pipeline/drone.hpp"
#include "yack/memory/allocator/dyadic.hpp"


namespace yack
{
    namespace concurrent
    {
        drone:: drone(const size_t size,
                      const size_t rank,
                      quark::threadable::procedure proc,
                      void                        *args) :
        next(0),
        prev(0),
        cond(),
        task(NULL),
        ctx(size,rank),
        thr(proc,args)
        {
        }

        drone:: ~drone() throw()
        {
        }


        drone *  drone:: zalloc( size_t &capa )
        {
            static memory::dyadic &mgr = memory::dyadic::instance();
            return static_cast<drone*>(mgr.acquire(capa,sizeof(drone)))-1;
        }
    }
}

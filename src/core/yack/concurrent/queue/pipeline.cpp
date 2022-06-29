#include "yack/concurrent/queue/pipeline.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace concurrent
    {
        pipeline:: worker:: worker(pipeline    &boss,
                                   const size_t size,
                                   const size_t rank) :
        next(0),
        prev(0),
        host(boss),
        cond(),
        task(NULL),
        ctx(size,rank),
        thr(pipeline::entry,&host)
        {
        }

        pipeline:: worker:: ~worker() throw()
        {
        }


        pipeline::worker * pipeline::worker:: zalloc( size_t &capa )
        {
            static memory::dyadic &mgr = memory::dyadic::instance();
            return static_cast<worker*>(mgr.acquire(capa,sizeof(worker)))-1;
        }
    }
}

namespace yack
{
    namespace concurrent
    {
        const char pipeline:: clid[] = "concurrent::pipeline";


        pipeline:: pipeline(const topology &topo) :
        sync(),
        gate(),
        threads(topo->size),
        zbytes_( threads ),
        squad( worker::zalloc(zbytes_) ),
        ready(0)
        {


            //______________________________________________________________
            //
            //
            // create
            //
            //______________________________________________________________
            {
                YACK_LOCK(sync);
                YACK_THREAD_PRINTLN(clid << "    <create #" << threads << ">");
            }
            pipeline &host    = *this;
            size_t    current = 0;
            while(current<threads)
            {
                try
                {
                    new ( &squad[current+1] ) worker(host,threads,current);
                    ++current;
                }
                catch(...)
                {
                    finish(current);
                }
            }

            {
                YACK_LOCK(sync);
                YACK_THREAD_PRINTLN(clid << "    all threads are launched...");
                for(size_t i=1;i<=threads;++i)
                {
                    const worker &w = squad[i];
                    assert(w.ctx.size==threads);
                    assert(w.ctx.indx==i);
                }
            }


            //______________________________________________________________
            //
            //
            // synchronize
            //
            //______________________________________________________________
            {
                sync.lock();
                if(ready<threads)
                {
                    YACK_THREAD_PRINTLN(clid << "    <still initializing>");
                    gate.wait(sync);
                    YACK_THREAD_PRINTLN(clid << "    <done initializing/>");
                    sync.unlock();
                }
                else
                {
                    YACK_THREAD_PRINTLN(clid << "    <already initialized/>");
                    sync.unlock();
                }
            }
            assert(threads==ready);

            //______________________________________________________________
            //
            //
            // placement
            //
            //______________________________________________________________
            try {
                assert(topo->size==threads);
                for(const quark::unode_type *node=topo->head;node;node=node->next)
                {
                    worker      &w = squad[node->indx];
                    const size_t j = node->core;
                    if(thread::verbose)
                    {
                        char who[32];
                        w.ctx.format(who,sizeof(who));
                        std::cerr << clid << "      ";
                        coerce(w.thr).assign(j,who);
                    }
                    else
                    {
                        coerce(w.thr).assign(j);
                    }
                }
            }
            catch(...)
            {
                finish(threads);
                throw;
            }

            YACK_THREAD_PRINTLN(clid << "    <create/>");
        }


        pipeline:: ~pipeline() throw()
        {
            YACK_THREAD_PRINTLN(clid << "    <leave>");
            finish(threads);
        }


        void pipeline:: zkill() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            mgr.withdraw(++squad,zbytes_);
        }

        void pipeline:: finish(size_t count) throw()
        {
            assert(count<=threads);

            zkill();
        }

        void pipeline:: entry(void *args) throw()
        {
            assert(NULL!=args);
            static_cast<pipeline *>(args)->cycle();
        }

        void pipeline:: cycle() throw()
        {
            // entering thread
            sync.lock(); assert(ready<threads);

            // get agent working in this thread
            worker &agent = squad[++ready];
            //YACK_THREAD_PRINTLN(clid << " #ready=" << ready);
            YACK_THREAD_PRINTLN(clid << " " << agent.ctx << " @ready=" << ready);

            if(ready>=threads) {
                gate.broadcast(); // synchronized
            }

            // waiting for job to do
            agent.cond.wait(sync);

        }


    }

}


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

namespace yack
{
    namespace concurrent
    {
        const char pipeline:: clid[] = "concurrent::pipeline";


        pipeline:: pipeline(const topology &topo) :
        sync(),
        threads(topo->size),
        available(),
        computing(),
        gate(),
        bytes( threads ),
        squad( drone::zalloc(bytes) ),
        ready(0)
        {


            //__________________________________________________________________
            //
            //
            // create
            //
            //__________________________________________________________________
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
                    new ( &squad[current+1] ) drone(host,threads,current);
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
            }


            //__________________________________________________________________
            //
            //
            // synchronize
            //
            //__________________________________________________________________
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

            //__________________________________________________________________
            //
            //
            // placement
            //
            //__________________________________________________________________
            try {
                assert(topo->size==threads);
                for(const quark::unode_type *node=topo->head;node;node=node->next)
                {
                    drone       &w = squad[node->indx];
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

            //__________________________________________________________________
            //
            //
            // initializing
            //
            //__________________________________________________________________
            for(size_t i=threads;i>0;--i)
            {
                drone *d = &squad[i];
                assert(d->ctx.size==threads);
                assert(d->ctx.indx==i);
                if( ran.choice() )
                {
                    available.push_back(d);
                }
                else
                {
                    available.push_front(d);
                }
            }
            assert(threads==available.size);



            YACK_THREAD_PRINTLN(clid << "    <create/>");
        }


        pipeline:: ~pipeline() throw()
        {
            YACK_THREAD_PRINTLN(clid << "    <leave>");

            available.restart();
            finish(threads);
        }


        void pipeline:: zkill() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            mgr.withdraw(++squad,bytes);
        }

        void pipeline:: finish(size_t count) throw()
        {
            assert(count<=threads);
            

            while(count>0)
            {
                --count;
            }
            zkill();
        }

        void pipeline:: entry(void *args) throw()
        {
            assert(NULL!=args);
            static_cast<pipeline *>(args)->cycle();
        }

        void pipeline:: cycle() throw()
        {
            //------------------------------------------------------------------
            //
            // entering thread
            //
            //------------------------------------------------------------------
            sync.lock(); assert(ready<threads);

            //------------------------------------------------------------------
            // get agent working in this thread
            //------------------------------------------------------------------
            drone &agent = squad[++ready];
            YACK_THREAD_PRINTLN(clid << " " << agent.ctx << " @ready=" << ready);

            //------------------------------------------------------------------
            // main thread sync
            //------------------------------------------------------------------
            if(ready>=threads) {
                gate.broadcast(); // synchronized
            }

            //------------------------------------------------------------------
            //
            // waiting for job to do on a LOCKED mutex => return UNLOCKED
            //
            //------------------------------------------------------------------
            agent.cond.wait(sync);

            // waking up on a locked mutex
            if(NULL == agent.task)
            {
                YACK_THREAD_PRINTLN(clid << " [done]");
                sync.unlock();
                return;
            }




        }


    }

}


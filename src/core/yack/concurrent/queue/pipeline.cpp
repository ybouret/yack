#include "yack/concurrent/queue/pipeline.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/concurrent/probe.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{
    namespace concurrent
    {
        const char pipeline:: clid[] = "[pipeline]";


        size_t pipeline:: size() const throw() { return threads; }


        pipeline::const_type &pipeline:: operator[](const size_t indx) const throw()
        {
            assert(indx>=1);
            assert(indx<=threads);
            return squad[indx].ctx;
        }


        pipeline:: pipeline(const topology &topo) :
        sync(),
        threads(topo->size),
        available(),
        computing(),
        pending(),
        zombies(),
        gate(),
        bytes( threads ),
        squad( drone::zalloc(bytes) ),
        ready(0),
        ran()
        {


            //__________________________________________________________________
            //
            //
            // create
            //
            //__________________________________________________________________
            {
                YACK_LOCK(sync);
                YACK_THREAD_PRINTLN(clid << " <create #" << threads << ">");
            }
            size_t    current = 0;
            while(current<threads)
            {
                try
                {
                    new ( &squad[current+1] ) drone(threads,current,entry,this);
                    ++current;
                }
                catch(...)
                {
                    // emergency probe to wait for ready>=current
                    YACK_LOCK_PROBE(sync,ready>=current);
                    finish(current);
                }
            }

            {
                YACK_LOCK(sync);
                YACK_THREAD_PRINTLN(clid << " [all threads are launched...]");
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
                    YACK_THREAD_PRINTLN(clid << "   <still initializing>");
                    gate.wait(sync);
                    YACK_THREAD_PRINTLN(clid << "   <done initializing/>");
                    sync.unlock();
                }
                else
                {
                    YACK_THREAD_PRINTLN(clid << "   <already initialized/>");
                    sync.unlock();
                }
            }
            assert(threads==ready);
            YACK_THREAD_PRINTLN(clid << " [...all threads are prepared]");

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
                        std::cerr << clid << "   -> ";
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
            // initializing the first queue of available drones
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


            YACK_THREAD_PRINTLN(clid << " <create/>");
        }


        pipeline:: ~pipeline() throw()
        {
            {
                YACK_LOCK(sync);
                YACK_THREAD_PRINTLN(clid << " <terminate>");
            }
            finish(threads);
            {
                YACK_LOCK(sync);
                YACK_THREAD_PRINTLN(clid << " <terminate/>");
            }
        }


        void pipeline:: zkill() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            mgr.withdraw(++squad,bytes);
        }

      
        void pipeline:: entry(void *args) throw()
        {
            assert(NULL!=args);
            static_cast<pipeline *>(args)->cycle();
        }

       
        job_uuid pipeline:: write(const job_type &J)
        {
            return process( zombies.query(J) );
        }



        bool pipeline:: ended(const job_uuid uuid) const throw()
        {
            YACK_LOCK( coerce(sync) );

            // look up in computing
            for(const drone *d=computing.head;d;d=d->next)
            {
                assert(d->task);
                if(uuid==d->task->uuid) return false; // found in computing
            }

            // look up in pending
            for(const jnode *node=pending.head;node;node=node->next)
            {
                if(uuid==node->uuid) return false; // found in pending
            }

            return true; // not found -> ended
        }

    }

}


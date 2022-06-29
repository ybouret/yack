#include "yack/concurrent/loop/simd.hpp"
#include "yack/type/utils.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/concurrent/thread.hpp"
#include <iostream>

namespace yack
{
    namespace concurrent
    {

        class simd::worker
        {
        public:
            inline ~worker() throw() {}


            worker(void        *args,
                   const size_t size,
                   const size_t rank) :
            ctx(size,rank),
            thr(simd::entry,args)
            {
            }



            const context ctx;
            const thread  thr;

            static inline
            worker *zalloc( size_t &capa )
            {
                static memory::dyadic &mgr = memory::dyadic::instance();
                return static_cast<worker*>(mgr.acquire(capa,sizeof(worker)));
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(worker);
        };

    }

}

#include "yack/system/hw.hpp"
#include "yack/concurrent/probe.hpp"

namespace yack
{
    namespace concurrent
    {
        void simd:: entry(void *args) throw()
        {
            assert(args);
            static_cast<simd *>(args)->cycle();
        }



        const char simd:: clid[] = "simd";


        simd:: simd(const topology &topo) :
        loop(),
        kcode(NULL),
        kargs(NULL),
        live(0),
        sync(clid),
        cond(),
        gate(),
        threads( max_of<size_t>(topo->size,1) ),
        zbytes_(threads),
        squad( worker::zalloc(zbytes_) ),
        ready(0)
        {
            //__________________________________________________________________
            //
            //
            // create threads
            //
            //__________________________________________________________________
            if(thread::verbose)
            {
                YACK_LOCK(sync);
                std::cerr << clid << "  <create_threads count=" << threads << ">" << std::endl;
            }

            {
                simd  &self    = *this;
                size_t current = 0;
                while(current<threads)
                {
                    try
                    {
                        new (&squad[current]) worker(&self,threads,current);
                        ++current;
                    }
                    catch(...)
                    {
                        // something bad happened
                        YACK_LOCK_PROBE(sync,ready>=current);
                        --squad;
                        finish(current);
                        throw;
                    }
                }
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
            try
            {
                assert(topo->size==threads);
                
                for(const quark::unode_type *node=topo->head;node;node=node->next)
                {
                    worker      &w = squad[node->rank];
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
                // something bad happened
                --squad;
                finish(threads);
                throw;
            }

            --squad;

            //__________________________________________________________________
            //
            //
            // done
            //
            //__________________________________________________________________
            if(thread::verbose)
            {
                YACK_LOCK(sync);
                std::cerr << clid << "  <create_threads/>" << std::endl;
            }
        }

        void simd:: zkill() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            mgr.withdraw(squad,zbytes_);
        }

        void simd:: finish(size_t count) throw()
        {
            assert(count<=threads);
            assert(ready>=count);
            assert(NULL==kcode);
            assert(NULL==kargs);

            if(thread::verbose)
            {
                YACK_LOCK(sync);
                std::cerr << clid << "  <terminating count=" << count << ">" << std::endl;
            }

            // wake up everyone to nothing to do
            cond.broadcast();

            // wait for each thread
            for(size_t t=count;t>0;--t)
            {
                destruct(&squad[t]);
            }

            if(thread::verbose)
            {
                YACK_LOCK(sync);
                std::cerr << clid << "  <terminating/>" << std::endl;
            }

            // remove resources
            ++squad;
            zkill();
        }


        simd:: ~simd() throw()
        {
            finish(threads);
        }


        size_t simd:: size() const throw()
        {
            return threads;
        }

        loop::const_type & simd:: operator[](const size_t indx) const throw()
        {
            assert(NULL!=squad);
            assert(indx>0);
            assert(indx<=threads);
            assert(squad[indx].ctx.indx==indx);
            return squad[indx].ctx;
        }

        lockable & simd::access() throw()
        {
            return sync;
        }

        const char * simd:: family() const throw()
        {
            return clid;
        }


        void simd:: cycle() throw()
        {
            //__________________________________________________________________
            //
            //
            // entering a thread: LOCK access
            //
            //__________________________________________________________________
            sync.lock(); assert(ready<threads);
            const context &here = squad[ready].ctx;
            YACK_THREAD_PRINTLN(clid << "    <started " << here << "/>");


            ++ready;
            if(ready>=threads)
            {
                // synchronized
                gate.broadcast();
            }

            //__________________________________________________________________
            //
            //
            // (first) wait on a LOCKED mutex
            //
            //__________________________________________________________________
        CYCLE:
            cond.wait(sync);


            //__________________________________________________________________
            //
            //
            // wake-up on a LOCKED mutex
            //
            //__________________________________________________________________

            if(kcode)
            {
                //______________________________________________________________
                //
                // wake up to WORK with live>0
                //______________________________________________________________
                assert(live>0);
                sync.unlock();
                kcode(here,kargs,sync);
                sync.lock();
                assert(live>0);
                if(--live<=0)
                {
                    gate.broadcast();
                }
                goto CYCLE;
            }

            //__________________________________________________________________
            //
            //
            // done
            //
            //__________________________________________________________________
            assert(ready>0);
            --ready;
            YACK_THREAD_PRINTLN(clid << "    <return " << here << "/>" );
            sync.unlock();

        }


    }

}

#include "yack/type/temporary.hpp"

namespace yack
{
    namespace concurrent
    {
        void simd:: operator()(kernel kcode_, void *kargs_) throw()
        {
            assert(NULL==kcode);
            assert(NULL!=kcode_);
            assert(NULL==kargs);
            assert(ready==threads);

            // threads are waiting for code on a LOCKED mutex
            const temporary<kernel> tempCode(kcode,kcode_);
            const temporary<void*>  tempArgs(kargs,kargs_);

            YACK_THREAD_PRINTLN(clid << "  <call>");


            // launch!
            live = threads;
            cond.broadcast();

            // wait
            sync.lock();
            if(live>0)
            {
                YACK_THREAD_PRINTLN(clid << "    <still live=" << live << "/>");
                gate.wait(sync);
            }
            YACK_THREAD_PRINTLN(clid << "  <call/>");
            sync.unlock();

        }

    }

}

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
            thread        thr;

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


        simd:: simd(const size_t n) :
        loop(),
        kcode(NULL),
        kargs(NULL),
        sync(clid),
        cond(),
        threads( max_of<size_t>(n,1) ),
        zbytes_(threads),
        team( worker::zalloc(zbytes_) ),
        ready(0),
        gate()
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
                std::cerr << "[simd]  <create_threads count=" << threads << ">" << std::endl;
            }

            {
                simd  &self    = *this;
                size_t current = 0;
                while(current<threads)
                {
                    try
                    {
                        new (&team[current]) worker(&self,threads,current);
                        ++current;
                    }
                    catch(...)
                    {
                        // something bad happened
                        zkill();
                        throw;
                    }
                }
            }

            //wtime chrono; chrono.wait(0.1);

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
                    YACK_THREAD_PRINTLN("[simd]    <still initializing>");
                    gate.wait(sync);
                    YACK_THREAD_PRINTLN("[simd]    <done initializing/>");
                    sync.unlock();
                }
                else
                {
                    YACK_THREAD_PRINTLN("[simd]    <already initialized/>");
                    sync.unlock();
                }
            }

            //__________________________________________________________________
            //
            //
            // finalize
            //
            //__________________________________________________________________

            assert(threads==ready);
            --team;
            if(thread::verbose)
            {
                YACK_LOCK(sync);
                std::cerr << "[simd]  <create_threads/>" << std::endl;
            }
        }

        void simd:: zkill() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            mgr.withdraw(team,zbytes_);
        }

        simd:: ~simd() throw()
        {
            assert(NULL==kcode);
            assert(NULL==kargs);

            if(thread::verbose)
            {
                YACK_LOCK(sync);
                std::cerr << "[simd]  <terminating count=" << threads << ">" << std::endl;
            }

            // wake up everyone to nothing to do
            cond.broadcast();

            // wait for each thread
            for(size_t t=threads;t>0;--t)
            {
                destruct(&team[t]);
            }

            if(thread::verbose)
            {
                YACK_LOCK(sync);
                std::cerr << "[simd]  <terminating/>" << std::endl;
            }

            // remove resources
            ++team;
            zkill();
        }


        size_t simd:: size() const throw()
        {
            return threads;
        }

        loop::const_type & simd:: operator[](const size_t indx) const throw()
        {
            assert(NULL!=team);
            assert(indx>0);
            assert(indx<=threads);
            assert(team[indx].ctx.indx==indx);
            return team[indx].ctx;
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
            const context &here = team[ready].ctx;
            YACK_THREAD_PRINTLN("[simd]    <started " << here << "/>");


            ++ready;
            if(ready>=threads)
            {
                // synchronize
                gate.broadcast();
            }

            //__________________________________________________________________
            //
            //
            // (first) wait on a LOCKED mutex
            //
            //__________________________________________________________________
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
                // wake up to WORK
                //______________________________________________________________
                sync.unlock();

                sync.lock();
            }

            //__________________________________________________________________
            //
            //
            // done
            //
            //__________________________________________________________________
            assert(ready>0);
            --ready;
            YACK_THREAD_PRINTLN("[simd]    <return " << here << "/>" );
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
            const temporary<kernel> tempCode(kcode,kcode_);
            const temporary<void*>  tempArgs(kargs,kargs_);
            




        }

    }

}

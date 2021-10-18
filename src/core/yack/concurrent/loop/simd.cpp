#include "yack/concurrent/loop/simd.hpp"
#include "yack/type/utils.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/concurrent/thread.hpp"
#include <iostream>
#include "yack/system/wtime.hpp"

namespace yack
{
    namespace concurrent
    {

        class simd::worker
        {
        public:
            ~worker() throw();
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

        void simd:: entry(void *args) throw()
        {
            assert(args);
            static_cast<simd *>(args)->cycle();
        }



        const char simd:: clid[] = "simd";





        simd:: simd(const size_t n) :
        loop(),
        sync(clid),
        cond(),
        ready(0),
        threads( max_of<size_t>(n,1) ),
        zbytes(threads),
        team( worker::zalloc(zbytes) )
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
                std::cerr << "[simd] create #thread=" << threads << std::endl;
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
                    YACK_THREAD_PRINTLN("[simd] still initializing...");
                    gate.wait(sync);
                    YACK_THREAD_PRINTLN("[simd] ...initialized!");
                    sync.unlock();
                }
                else
                {
                    YACK_THREAD_PRINTLN("[simd] already initialized");
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
        }

        void simd:: zkill() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            mgr.withdraw(team,zbytes);
        }

        simd:: ~simd() throw()
        {
            if(thread::verbose)
            {
                YACK_LOCK(sync);
                std::cerr << "[simd] terminating..." << std::endl;
            }

            while(ready>0)
            {

            }
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
            if(thread::verbose)
            {
                std::cerr << "[simd] in thread " << here.size << "." << here.indx << std::endl;
            }

            ++ready;
            if(ready>=threads)
            {
                // synchronize
                gate.broadcast();
            }

            //__________________________________________________________________
            //
            //
            // fist wait on a LOCKED mutex
            //
            //__________________________________________________________________
            cond.wait(sync);
            



        }

    }

}

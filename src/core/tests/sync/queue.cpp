#include "yack/concurrent/queue/agent.hpp"
#include "yack/concurrent/context.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/dyadic.hpp"


using namespace yack;


namespace yack
{
    namespace concurrent
    {

        class pipeline
        {
        public:
            static const char clid[];

            class worker : public context
            {
            public:
                inline worker(pipeline    &boss,
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

                inline ~worker() throw()
                {
                }


                worker       *next; //!< for lists
                worker       *prev; //!< for lists
                pipeline     &host; //!< origin
                condition     cond; //!< auto-condition
                void         *task; //!< task to do
                const context ctx;  //!< context
                const thread  thr;  //!< thread

                static inline
                worker *zalloc( size_t &capa )
                {
                    static memory::dyadic &mgr = memory::dyadic::instance();
                    return static_cast<worker*>(mgr.acquire(capa,sizeof(worker)));
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(worker);
            };

            explicit pipeline(const topology &topo) :
            sync(),
            gate(),
            threads(topo->size),
            zbytes_( threads ),
            squad( worker::zalloc(zbytes_) ),
            ready(0)
            {
                if(thread::verbose)
                {
                    std::cerr << clid << " zbytes=" << zbytes_ << std::endl;
                }

                {
                    pipeline &host = *this;
                    size_t    current = 0;
                    while(current<threads)
                    {
                        try
                        {
                            new ( &squad[current] ) worker(host,threads,current);
                            ++current;
                        }
                        catch(...)
                        {
                            --squad;
                            finish(current);
                        }
                    }
                }

            PROBE:
                if( sync.try_lock() )
                {
                    if(ready<threads)
                    {
                        std::cerr << "not ready..." << std::endl;
                        sync.unlock();
                        goto PROBE;
                    }
                    sync.unlock();
                }
                std::cerr << "ready!" << std::endl;
                

                // done
                --squad;
            }

            virtual ~pipeline() throw()
            {
                std::cerr << clid << " done" << std::endl;
                finish(threads);
            }

            mutex     sync;
            condition gate;
            const size_t threads;
            size_t       zbytes_;
            worker      *squad;
            size_t       ready;

            void        cycle()       throw();
            static void entry(void *) throw();
            void        zkill() throw();
            void        finish(size_t count) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pipeline);
        };

        const char pipeline:: clid[] = "concurrent::pipeline";

        void pipeline:: zkill() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            mgr.withdraw(squad,zbytes_);
        }

        void pipeline:: finish(size_t count) throw()
        {
            assert(count<=threads);
            ++squad;
            zkill();
        }

        void pipeline:: entry(void *args) throw()
        {
            assert(NULL!=args);
            static_cast<pipeline *>(args)->cycle();
        }

        void pipeline:: cycle() throw()
        {
            sync.lock();
            assert(ready<threads);
            worker &agent = squad[ready++];

            { std::cerr << clid << " " << agent.ctx << " @ready=" << ready << std::endl; }

            agent.cond.wait(sync);

        }

    }


}

YACK_UTEST(sync_queue)
{
    //YACK_SIZEOF(concurrent::agent);
    const concurrent::topology topo;
    concurrent::thread::verbose = true;

    std::cerr << "topo=" << topo << std::endl;
    concurrent::pipeline Q(topo);

}
YACK_UDONE()

#include "yack/concurrent/queue/agent.hpp"
#include "yack/concurrent/context.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/utest/run.hpp"


using namespace yack;


namespace yack
{
    namespace concurrent
    {

        class pipeline
        {
        public:
            class worker : public context
            {
            public:
                inline worker(pipeline    &boss,
                              const size_t size,
                              const size_t rank) :
                next(0),
                prev(0),
                host(boss),
                ctx(size,rank),
                thr(pipeline::entry,this)
                {
                }

                worker       *next;
                worker       *prev;
                pipeline     &host;
                const context ctx;
                thread        thr;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(worker);
            };

            explicit pipeline(const topology &);
            virtual ~pipeline() throw() {}

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


    }


}

YACK_UTEST(sync_queue)
{
    //YACK_SIZEOF(concurrent::agent);
    const concurrent::topology topo;

    std::cerr << "topo=" << topo << std::endl;

}
YACK_UDONE()

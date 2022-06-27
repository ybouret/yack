#include "yack/concurrent/queue/agent.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/utest/run.hpp"


using namespace yack;


namespace yack
{
    namespace concurrent
    {
        class agency
        {
        public:
            size_t               live;
            mutex                sync;
            condition            cond;
            cxx_array<condition> halt;
            cxx_series<thread>   crew;

            virtual ~agency() throw()
            {
            }

            explicit agency(const concurrent::topology &topo) :
            live(0),
            sync(),
            cond(),
            halt(topo->size),
            crew(topo->size)
            {

                const size_t dims = topo->size;
                while(crew.size()<dims)
                {
                    crew.add<thread::procedure,void*>(launch,this);
                }
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(agency);

            static void launch(void *args) throw()
            {
                assert(args);
                static_cast<agency *>(args)->mission();
            }

            void mission() throw()
            {
                sync.lock();
                ++live; std::cerr << "creating mission @" << live << std::endl;
                assert(live<halt.size());
                condition &waiting = halt[live];
                waiting.wait(sync);
            }



        };

    }


}

YACK_UTEST(sync_queue)
{
    YACK_SIZEOF(concurrent::agent);
    const concurrent::topology topo;

    std::cerr << "topo=" << topo << std::endl;
    concurrent::agency  A( topo );
    
}
YACK_UDONE()

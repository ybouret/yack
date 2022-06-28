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
            virtual ~agency() throw() {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(agency);
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

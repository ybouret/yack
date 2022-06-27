#include "yack/concurrent/queue/agent.hpp"
#include "yack/utest/run.hpp"


using namespace yack;


namespace yack
{
    namespace concurrent
    {
        class agency
        {
        public:
            virtual ~agency() throw()
            {
            }

            explicit agency(const concurrent::topology &topo)
            {

            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(agency);
        };

    }


}

YACK_UTEST(sync_queue)
{
    YACK_SIZEOF(concurrent::agent);
    const concurrent::topology topo(1);

    std::cerr << "topo=" << topo << std::endl;
    
}
YACK_UDONE()

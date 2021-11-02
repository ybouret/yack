
#include "yack/concurrent/queue/agent.hpp"
#include "yack/concurrent/mutex.hpp"

namespace yack
{
    namespace concurrent
    {
        agent:: ~agent() throw()
        {
        }

        agent:: agent(const size_t uniqID,
                      mutex       &access,
                      size_t      &run_up) :
        next(0),
        prev(0),
        uuid(uniqID),
        sync(access),
        left(run_up),
        cond(),
        self(agent::call,this)
        {
            assert(uuid>0);

        }

        void agent::call(void *args) throw()
        {
            assert(args);
            static_cast<agent *>(args)->mission();
        }

        void agent:: mission() throw()
        {

            // call from thread: LOCK mutex
            sync.lock();
            assert(left>0);

            --left;

            // wait on a locked mutex
            cond.wait(sync);

            // wake up on a locked mutex
            sync.unlock();


        }
        

        
    }

}


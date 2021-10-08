
#include "yack/concurrent/thread.hpp"
#include "yack/concurrent/mutex.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/concurrent/singleton.hpp"

#include "yack/utest/run.hpp"
#include <cstring>
using namespace yack;

namespace
{
    class workspace
    {
    public:
        concurrent::mutex     access;
        concurrent::condition cv;
        size_t                ready;

        inline workspace() : access("workspace"), cv(), ready()
        {
            std::cerr << "+workspace" << std::endl;
        }

        inline ~workspace() throw()
        {
            std::cerr << "-workspace" << std::endl;
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(workspace);
    };

    static inline void run(void *args) throw()
    {
        assert(args);
        workspace &wksp = *static_cast<workspace *>(args);
        wksp.access.lock();
        const size_t label = ++wksp.ready;
        std::cerr << "Run with ready=" << wksp.ready << std::endl;
        std::cerr.flush();

        // wait on a locked mutex
        wksp.cv.wait(wksp.access);

        // wake up on a locked mutex
        std::cerr << "just woke at label=" << label << std::endl;
        std::cerr.flush();
        wksp.access.unlock();
    }

}

YACK_UTEST(sync_thread)
{
    concurrent::singleton::verbose = true;
    concurrent::mutex::verbose     = true;
    workspace          wksp;
    {
        concurrent::thread t1(run,&wksp);
        concurrent::thread t2(run,&wksp);
        concurrent::thread t3(run,&wksp);
        bool sync = false;
        while(!sync)
        {
            if(!wksp.access.try_lock())
            {
                continue;
            }
            if(wksp.ready>=3)
            {
                sync = true;
            }
            wksp.access.unlock();
        }
        std::cerr << "in main..." << std::endl;
        wksp.cv.signal();
        wksp.cv.broadcast();
    }
    

}
YACK_UDONE()


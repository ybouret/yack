
#include "yack/concurrent/queue/agent.hpp"
#include "yack/concurrent/mutex.hpp"



namespace yack
{
    namespace concurrent
    {

        agent:: agent(mutex  &shared_sync,
                      size_t &shared_ready) :
        next(0),
        prev(0),
        sync(shared_sync),
        cond(),
        ready(shared_ready),
        indx(0),
        thr(enroll,this)
        {
        }

        agent:: ~agent() throw()
        {

        }

        void agent:: enroll(void *args) throw()
        {
            assert(args);
            static_cast<agent *>(args)->mission();
        }

        void agent:: mission() throw()
        {
            // entering missing
            sync.lock();
            coerce(indx) = ++ready;
            std::cerr << "agent #" << indx << std::endl;
            std::cerr.flush();

            // first wait
            cond.wait(sync);

        }

    }

}


namespace yack
{
    namespace concurrent
    {
        agency:: ~agency() throw()
        {
        }

        agency:: agency(const size_t n) :
        crew(n),
        sync(),
        ready(0)
        {
            assert(n>0);

            try
            {
                for(size_t i=1;i<=n;++i)
                {
                    crew.add<mutex&,size_t&>(sync,ready);
                }

                
            }
            catch(...)
            {

                throw;
            }

        }
        
    }

}


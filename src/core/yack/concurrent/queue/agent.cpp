
#include "yack/concurrent/queue/agent.hpp"
#include "yack/concurrent/mutex.hpp"



namespace yack
{
    namespace concurrent
    {

        agent:: agent(size_t    &shared_live,
                      mutex     &shared_sync,
                      condition &shared_comm) :
        next(0),
        prev(0),
        live(shared_live),
        sync(shared_sync),
        comm(shared_comm),
        cond(),
        indx(0),
        thr(enroll,this)
        {
        }

        agent:: ~agent() throw()
        {
            {
                YACK_LOCK(sync);
                std::cerr << "~agent#" << indx << std::endl;
            }
            cond.signal();
        }

        void agent:: enroll(void *args) throw()
        {
            assert(args);
            static_cast<agent *>(args)->mission();
        }

        void agent:: mission() throw()
        {
            // entering mission
            sync.lock();
            coerce(indx) = ++live;
            std::cerr << "agent #" << indx << std::endl;
            std::cerr.flush();

            //
            comm.broadcast();

            // first wait
            cond.wait(sync);

            // wake up on a locked mutex
            std::cerr << "wake up #" << indx << std::endl;

            --live;
            sync.unlock();
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
        sync(),
        comm(),
        live(0),
        crew(n)
        {
            assert(n>0);

            try
            {
                for(size_t i=1;i<=n;++i)
                {
                    crew.add<size_t&,mutex&,condition&>(live,sync,comm);
                    sync.lock();
                    if(i!=live)
                    {
                        std::cerr << "waiting for #" << i << std::endl;
                        comm.wait(sync);
                    }
                    sync.unlock();
                }

                std::cerr << "synchronized!" << std::endl;


            }
            catch(...)
            {
                throw;
            }

        }
        
    }

}


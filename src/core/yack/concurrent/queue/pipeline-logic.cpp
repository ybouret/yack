
#include "yack/concurrent/queue/pipeline.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/concurrent/probe.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{
    namespace concurrent
    {
        void pipeline:: finish(size_t count) throw()
        {
            assert(count<=threads);
            assert(ready>=count);

            //------------------------------------------------------------------
            //
            // remove further jobs
            //
            //------------------------------------------------------------------
            sync.lock();
            while(pending.size) zombies.store( pending.pop_back() );

            //------------------------------------------------------------------
            //
            // waiting for computing to end
            //
            //------------------------------------------------------------------
            if(computing.size)
            {
                gate.wait(sync);
            }
            sync.unlock();



            //------------------------------------------------------------------
            //
            // shutdown all available
            //
            //------------------------------------------------------------------
            assert(count==available.size);
            assert(ready==available.size);
            while(available.size)
            {
                assert(NULL == available.tail->task);
                available.pop_back()->cond.signal();
            }

            //------------------------------------------------------------------
            //
            // waiting for all threads to return
            //
            //------------------------------------------------------------------
            {
                sync.lock();
                if(ready>0)
                {
                    YACK_THREAD_PRINTLN(clid << "   <still finishing>");
                    gate.wait(sync);
                    YACK_THREAD_PRINTLN(clid << "   <done finishing/>");
                    sync.unlock();
                }
                else
                {
                    YACK_THREAD_PRINTLN(clid << "   <already finished/>");
                    sync.unlock();
                }
            }

            //------------------------------------------------------------------
            //
            // cleaning up memory
            //
            //------------------------------------------------------------------
            while(count>0)
            {
                out_of_reach::zset( destructed(&squad[count--]), sizeof(drone) );
            }
            zkill();
        }

    }

}


namespace yack
{
    namespace concurrent
    {
        void pipeline:: cycle() throw()
        {
            //------------------------------------------------------------------
            //
            // entering thread
            //
            //------------------------------------------------------------------
            sync.lock(); assert(ready<threads);

            //------------------------------------------------------------------
            // get agent working in this thread
            //------------------------------------------------------------------
            drone &agent = squad[++ready];
            YACK_THREAD_PRINTLN(clid << " " << agent.ctx << " @ready=" << ready);

            //------------------------------------------------------------------
            // main thread sync
            //------------------------------------------------------------------
            if(ready>=threads) {
                gate.broadcast(); // synchronized
            }

            //------------------------------------------------------------------
            //
            // waiting for job to do on a LOCKED mutex => return UNLOCKED
            //
            //------------------------------------------------------------------
        CYCLE:
            agent.cond.wait(sync);

            //------------------------------------------------------------------
            //
            // waking up on a LOCKED mutex
            //
            //------------------------------------------------------------------
            if(NULL == agent.task)
            {
                //--------------------------------------------------------------
                // done!
                //--------------------------------------------------------------
                assert(ready>0);
                YACK_THREAD_PRINTLN(clid << " " << agent.ctx << " [done]");
                --ready;
                if(ready<=0) gate.broadcast();
                sync.unlock();
                return;
            }
            else
            {
                //--------------------------------------------------------------
                // work!
                //--------------------------------------------------------------
                YACK_THREAD_PRINTLN(clid << " " << agent.ctx << " [work]");
                sync.unlock();
                agent.task->call(sync);

                sync.lock();
                goto CYCLE;
            }


        }

    }

}

namespace yack
{
    namespace concurrent
    {
        job_uuid pipeline:: process(jNode *alive) throw()
        {
            YACK_LOCK(sync);
            assert(NULL!=alive);

            if(available.size)
            {
                //--------------------------------------------------------------
                //
                // a drone is available: load it and execute
                //
                //--------------------------------------------------------------
                drone *d = computing.push_back( available.pop_back() ); // available -> computing
                d->task  = alive;
                YACK_THREAD_PRINTLN(clid << " " << d->ctx << " process $" << alive->uuid);
                d->cond.signal();
            }
            else
            {
                YACK_THREAD_PRINTLN(clid << " postpone $" << alive->uuid);
                pending.push_back(alive);
            }


            return alive->uuid;
        }
    }

}


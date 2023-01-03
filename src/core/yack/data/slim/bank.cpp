#include "yack/data/slim/bank.hpp"
#include "yack/concurrent/mutex.hpp"

namespace yack
{
    namespace kernel
    {
        const char * const slim_bank:: clid    = "slim_bank";
        bool &             slim_bank:: verbose = concurrent::mutex::verbose;

        slim_bank:: ~slim_bank() throw()
        {
            assert(sync);
            object::zrelease( destructed(sync) );
        }

        slim_bank:: slim_bank() :
        object(), counted(), releasable(), sync( object::zacquire<concurrent::mutex>() )
        {
            try {
                new (sync) concurrent::mutex(clid);
            }
            catch(...)
            {
                object::zrelease(sync);
                throw;
            }
        }

        lockable & slim_bank:: operator*() throw()
        {
            assert(sync);
            return *sync;
        }

    }
}

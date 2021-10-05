
#include "yack/synchronic/mutex.hpp"
#include "yack/synchronic/quark/mutex.hpp"

namespace yack
{
    namespace synchronic
    {
        mutex:: mutex() :
        primitive(), lockable(),
        impl( quark::mutex_api::init() )
        {

        }

        mutex:: ~mutex() throw()
        {
            assert(impl);
            quark::mutex_api::quit(impl);
            assert(!impl);
        }

        void mutex:: lock() throw()
        {
            assert(impl);
            quark::mutex_api::lock(impl);
        }


        void mutex:: unlock() throw()
        {
            assert(impl);
            quark::mutex_api::unlock(impl);
        }

        bool mutex:: try_lock() throw()
        {
            assert(impl);
            return quark::mutex_api::try_lock(impl);
        }
    }

}


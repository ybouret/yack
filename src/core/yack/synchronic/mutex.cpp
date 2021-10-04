
#include "yack/synchronic/mutex.hpp"
#include "yack/synchronic/quark/mutex.hpp"

namespace yack
{
    namespace synchronic
    {
        mutex:: mutex() : impl( quark::mutex_create() )
        {

        }

        mutex:: ~mutex() throw()
        {
            assert(impl);
            quark::mutex_delete(impl);
            impl = NULL;
        }

        void mutex:: lock() throw()
        {
            assert(impl);
            quark::mutex_lock(impl);
        }


        void mutex:: unlock() throw()
        {
            assert(impl);
            quark::mutex_unlock(impl);
        }

        bool mutex:: try_lock() throw()
        {
            assert(impl);
            return quark::mutex_try_lock(impl);
        }
    }

}


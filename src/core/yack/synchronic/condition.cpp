#include "yack/synchronic/condition.hpp"
#include "yack/synchronic/quark/condition.hpp"
#include "yack/synchronic/mutex.hpp"

namespace yack
{
    namespace synchronic
    {
        condition:: condition() :
        primitive(),
        impl( quark::condition_create() )
        {
            assert(impl);
        }
        
        
        condition:: ~condition() throw()
        {
            assert(NULL!=impl);
            quark::condition_delete(impl);
            impl=NULL;
        }

        void condition:: wait(mutex &m) throw()
        {
            assert(NULL!=impl);
            quark::condition_wait(impl,m.impl);
        }

        void condition:: signal() throw()
        {
            assert(NULL!=impl);
            quark::condition_signal(impl);
        }

        void condition:: broadcast() throw()
        {
            assert(NULL!=impl);
            quark::condition_broadcast(impl);
        }
        
    }
    
}

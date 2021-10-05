

#include "yack/synchronic/quark/condition.hpp"
#include "yack/synchronic/condition.hpp"
#include "yack/synchronic/mutex.hpp"

namespace yack
{
    namespace synchronic
    {
        condition:: condition() :
        primitive(),
        impl( quark::condition_api::create() )
        {
            assert(impl);
        }


        condition:: ~condition() throw()
        {
            assert(NULL!=impl);
            quark::condition_api:: destruct(impl);
            impl=NULL;
        }


        void condition:: signal() throw()
        {
            assert(impl);
            impl->signal();
        }

        void condition:: broadcast() throw()
        {
            assert(impl);
            impl->broadcast();
        }

        void condition:: wait(mutex &m) throw()
        {
            assert(impl);
            impl->wait(m.impl);
        }

        namespace quark
        {

            condition *condition_api::create()
            {
                static atelier &mgr = atelier_instance();
                return mgr.conditions.invoke<condition>();
            }

            void condition_api:: destruct(condition *cond) throw()
            {
                assert(NULL!=cond);
                static atelier &mgr = atelier_location();
                return mgr.conditions.revoke(cond);
            }

            
        }

    }

}



#include "yack/concurrent/quark/condition.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/concurrent/mutex.hpp"

namespace yack
{
    namespace concurrent
    {
        condition:: condition() :
        primitive(),
        impl( quark::condition_api::init() )
        {
            assert(impl);
        }


        condition:: ~condition() throw()
        {
            assert(NULL!=impl);
            quark::condition_api::quit(impl);
            assert(NULL==impl);
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

            condition *condition_api:: init()
            {
                static atelier &mgr = atelier_instance();
                return mgr.conditions.invoke<condition>();
            }

            void condition_api:: quit(condition * &cond) throw()
            {
                assert(NULL!=cond);
                static atelier &mgr = atelier_location();
                mgr.conditions.revoke(cond);
                cond = NULL;
            }

            
        }

    }

}

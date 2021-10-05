
#include "yack/synchronic/quark/condition.hpp"
namespace yack
{
    namespace synchronic
    {
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

            void condition_api:: wait(condition *cond,mutex *m) throw()
            {
                assert(cond);
                assert(m);
                cond->wait(m);
            }

            void condition_api:: signal(condition *cond)   throw()
            {
                assert(cond);
                cond->signal();
            }

            void condition_api:: broadcast(condition *cond) throw()
            {
                assert(cond);
                cond->broadcast();
            }

            
        }

    }

}



#include "yack/concurrent/quark/condition.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/concurrent/mutex.hpp"

namespace yack
{
    namespace concurrent
    {
        condition:: condition() :
        primitive(),
        cond( quark::condition_api::init() )
        {
            assert(NULL!=cond);
        }


        condition:: ~condition() noexcept
        {
            assert(NULL!=cond);
            quark::condition_api::quit(cond);
            assert(NULL==cond);
        }


        void condition:: signal() noexcept
        {
            assert(cond);
            cond->signal();
        }

        void condition:: broadcast() noexcept
        {
            assert(cond);
            cond->broadcast();
        }

        void condition:: wait(mutex &m) noexcept
        {
            assert(NULL!=cond);
            if(mutex::verbose)
            {
                std::cerr << "waiting on <" << m.name << ">" << std::endl;
            }
            cond->wait(m.impl);
        }

        namespace quark
        {

            condition *condition_api:: init()
            {
                static atelier &mgr = atelier_instance();
                return mgr.conditions.invoke<condition>();
            }

            void condition_api:: quit(condition * &cond) noexcept
            {
                assert(NULL!=cond);
                static atelier &mgr = atelier_location();
                mgr.conditions.revoke(cond);
                cond = NULL;
            }

            
        }

    }

}

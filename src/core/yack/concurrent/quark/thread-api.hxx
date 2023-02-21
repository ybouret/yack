
namespace yack
{
    namespace concurrent
    {
        namespace quark
        {
            thread *thread_api::init(threadable &exe)
            {
                static atelier &mgr = atelier_instance();
                thread         *thr = mgr.threads.zombie<thread>();
                try {
                    return new(thr) thread(exe);
                }
                catch(...)
                {
                    mgr.threads.release(thr);
                    throw;
                }
            }

            void thread_api:: quit(thread * &thr) noexcept
            {
                assert(NULL!=thr);
                static atelier &mgr = atelier_location();
                mgr.threads.expunge( destructed(thr) );
                thr = NULL;
            }

            
        }

    }

}





#include "yack/concurrent/thread.hpp"

namespace yack
{
    namespace concurrent
    {

        bool thread::verbose = false;

        thread:: thread(threadable::procedure proc, void *args) :
        threadable(proc,args),
        impl( quark::thread_api::init(*this) )
        {
            assert(NULL!=impl);
        }

        thread:: ~thread() noexcept
        {
            assert(NULL!=impl);
            quark::thread_api::quit(impl);
            assert(NULL==impl);
        }

        static inline void display_who(const size_t cpu, const char *who)
        {
            assert(who);
            std::cerr << "[thread " << who << "]@cpu " << cpu << std::endl;
        }

        void thread:: assign(const size_t cpu, const char *who)
        {
            assert(NULL!=impl);
            quark::thread::assign(impl->self,cpu);
            if(who) display_who(cpu,who);
        }

        void thread:: assign_current(const size_t cpu, const char *who)
        {
            quark::thread::assign( quark::thread::get_current_handle(), cpu );
            if(who) display_who(cpu,who);
        }

    }

}

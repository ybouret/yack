
#if defined YACK_THREAD_AFFINITY
#error "YACK_THREAD_AFFINITY shouldn't be defined"
#endif

#if defined(YACK_DARWIN)
#define YACK_THREAD_AFFINITY 1
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
namespace yack
{
    namespace concurrent
    {
        namespace quark
        {
            void thread:: assign(handle h, const size_t j)
            {
                thread_affinity_policy_data_t policy_data = { int(j) };
                mach_port_t                   mach_thread = pthread_mach_thread_np(h);
                const int                     mach_result = thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy_data, THREAD_AFFINITY_POLICY_COUNT);
                if( KERN_SUCCESS != mach_result )
                {
                    throw mach::exception(mach_result,"thread_policy_set");
                }
            }
        }
    }
}
#endif

#if defined(YACK_LINUX)|| defined(YACK_FREEBSD)
#   define YACK_THREAD_AFFINITY 1
#   if defined(YACK_FREEBSD)
#      include <pthread_np.h>
#      define YACK_CPU_SET cpuset_t
#   else
#      define YACK_CPU_SET cpu_set_t
#endif
namespace yack
{
    namespace concurrent
    {
        namespace quark
        {
            void   thread:: assign(handle h,const size_t j)
            {
                YACK_CPU_SET the_cpu_set;
                CPU_ZERO(  &the_cpu_set );
                CPU_SET(j, &the_cpu_set );
                const int err = pthread_setaffinity_np( h, sizeof(YACK_CPU_SET), &the_cpu_set );
                if( err != 0 )
                    throw libc::exception( err, "pthread_setaffinity_np" );
            }
        }
    }
}
#endif



#if defined(YACK_WIN)
#define YACK_THREAD_AFFINITY 1
namespace yack
{
    namespace concurrent
    {
        namespace quark
        {
            void thread:: assign( handle h, const size_t j )
            {
                const DWORD_PTR mask = DWORD_PTR(1) << j;
                if( ! ::SetThreadAffinityMask( h, mask ) )
                {
                    const DWORD err = ::GetLastError();
                    throw win32::exception( err, "::SetThreadAffinityMask" );
                }
            }

        }
    }
}
#endif


#if defined(YACK_SUNOS)
#include <sys/types.h>
#include <sys/processor.h>
#include <sys/procset.h>
#define YACK_THREAD_AFFINITY 1

namespace yack
{
    namespace concurrent
    {
        namespace quark
        {
            void thread::assign(handle h, const size_t j, const char *who)
            {
                const int res = processor_bind(P_LWPID,idtype_t(h),j,NULL);
                if(0!=res) throw exception("processor_bind failure");
            }
        }

    }

}

#endif

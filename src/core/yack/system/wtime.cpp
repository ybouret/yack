#include "yack/system/wtime.hpp"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"

#if defined(YACK_DARWIN)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#if defined(YACK_LINUX) || defined(YACK_FREEBSD) || defined(YACK_SUNOS) || defined(YACK_OPENBSD)
#define YACK_USE_CLOCK_GETTIME 1
#include <time.h>
#include <sys/time.h>
#include <cerrno>
#endif

#if defined(YACK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


namespace yack
{

#if defined(YACK_DARWIN)
    uint64_t wtime:: ticks()
    {
        YACK_GIANT_LOCK();
        return mach_absolute_time();
    }

    static inline double wtime_calibrate()
    {
        YACK_GIANT_LOCK();
        mach_timebase_info_data_t timebase;
        const kern_return_t err = mach_timebase_info(&timebase);
        if(KERN_SUCCESS !=err)
        {
            throw imported::exception("mach_timebase_info","%s",mach_error_string(err));
        }
        double conversion_factor = double(timebase.numer) / timebase.denom;
        return 1e-9 * conversion_factor;
    }
#endif

#if defined(YACK_USE_CLOCK_GETTIME)
    static const uint64_t __giga64 = YACK_U64(0x3B9ACA00);

    static inline uint64_t wtime_calibrate()
    {
        YACK_GIANT_LOCK();
        struct timespec tp  = { 0, 0 };
        const int       err = clock_getres(CLOCK_REALTIME,&tp);
        if(err!=0) throw libc::exception( errno, "clock_getres" );
        return __giga64*uint64_t(tp.tv_sec) + uint64_t(tp.tv_nsec);
    }

    uint64_t wtime:: ticks()
    {
        YACK_GIANT_LOCK();
        struct timespec tp  = { 0, 0 };
        const int       err = clock_gettime( CLOCK_REALTIME, &tp );
        if(err!=0) throw libc::exception( errno, "clock_gettime" );
        return __giga64*uint64_t(tp.tv_sec) + uint64_t(tp.tv_nsec);
    }

#endif

#if defined(YACK_WIN)
    static inline double wtime_calibrate()
    {
        static const long double l_one = 1;
        YACK_GIANT_LOCK();
        LARGE_INTEGER F;
        if( ! :: QueryPerformanceFrequency( &F ) )
        {
            throw win32::exception( ::GetLastError(), "::QueryPerformanceFrequency" );
        }
        return static_cast<double>(l_one / static_cast<long double>( F.QuadPart ));
    }

    uint64_t real_time_clock:: ticks()
    {
        YACK_GIANT_LOCK();
        int64_t Q = 0;
        if( ! ::QueryPerformanceCounter( (LARGE_INTEGER *)&Q)  )
        {
            throw win32::exception( ::GetLastError(), " ::QueryPerformanceCounter" );
        }
        return uint64_t(Q);
    }
    
#endif


    wtime:: wtime() : freq( wtime_calibrate() )
    {
    }

    wtime:: ~wtime() throw()
    {
        coerce(freq) = 0;
    }

    

}

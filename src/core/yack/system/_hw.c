
#include "yack/system/setup.h"

#if defined(YACK_DARWIN)
#include <sys/param.h>
#include <sys/sysctl.h>
#endif

#if defined(YACK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(YACK_LINUX) || defined(YACK_FREEBSD) || defined(YACK_SUNOS) || defined(YACK_OPENBSD)
#include <unistd.h>
#endif

size_t upsylon_hardware_nprocs()
{

#if defined(YACK_DARWIN)
    int      nm[2] = { CTL_HW, HW_AVAILCPU };
    size_t   len = 4;
    uint32_t count=0;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) { count = 1; }
    }
    return count;
#endif
    
#if defined(YACK_WIN)
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    const DWORD n = sysinfo.dwNumberOfProcessors;
    return n <= 1 ? 1:n;
#endif

#if defined(YACK_LINUX) || defined(YACK_FREEBSD) || defined(YACK_SUNOS) || defined(YACK_OPENBSD)
    const long n = sysconf( _SC_NPROCESSORS_ONLN );
    return n <= 0  ? 1 : n;
#endif


}

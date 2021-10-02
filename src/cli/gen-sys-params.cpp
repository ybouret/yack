#include "yack/setup.hpp"
#include "yack/system/platform.h"
#include "yack/system/arch.h"

#include <iostream>

#if defined(YACK_BSD)
#include <pthread.h>
void findParameters()
{
    std::cerr << "sizeof(pthread_mutex) = " << sizeof(pthread_mutex_t) << std::endl;
}
#endif

#if defined(YACK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
void findParameters()
{
	std::cerr << "sizeof(CriticalSection) = " << sizeof(CRITICAL_SECTION) << std::endl;
}
#endif

#if !defined(YACK_SYSTEM_PARAMS)
#error "Need YACK_SYSTEM_PARAMS"
#endif

int main()
{

    std::cerr << YACK_SYSTEM_PARAMS << std::endl;
    std::cerr << "Platform : " << YACK_PLATFORM << std::endl;
    std::cerr << "Arch     : " << YACK_ARCHITECTURE << std::endl;
    findParameters();

    return 0;
}




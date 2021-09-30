#include "yack/setup.hpp"
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

int main()
{

	findParameters();

    return 0;
}




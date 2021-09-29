#include "yack/setup.hpp"
#include <iostream>

#if defined(YACK_BSD)
#include <pthread.h>
void findParameters()
{
    std::cerr << "sizeof(pthread_mutex) = " << sizeof(pthread_mutex_t) << std::endl;
}
#endif

int main(int argc, const char *argv[])
{
    findParameters();

    return 0;
}




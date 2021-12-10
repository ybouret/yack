#include "yack/system/env.hpp"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"
#include <iostream>

#if defined(YACK_BSD)
#include <unistd.h>
extern "C" char **environ;
#endif

namespace yack
{

    void environment::show()
    {
        YACK_GIANT_LOCK();
#if defined(YACK_BSD)
        for(char **env=environ;*env;++env)
        {
            std::cerr << "[" << *env << "]" << std::endl;
        }
#endif

#if defined(YACK_WIN)
        LPTCH lpvEnv = ::GetEnvironmentStrings();
#endif


    }

}


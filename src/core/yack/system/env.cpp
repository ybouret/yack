#include "yack/system/env.hpp"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"
#include <iostream>

#if defined(YACK_BSD)
#include <unistd.h>
extern "C" char **environ;
#endif

#if defined(YACK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


namespace yack
{

    namespace
    {
#if defined(YACK_WIN)
        class ESQuery
        {
        public:
            inline ESQuery()
            {
            }

            inline ~ESQuery() throw()
            {
            }
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ESQuery);
        };
#endif

    }

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
        if(!lpvEnv) throw win32::exception( ::GetLastError(), "GetEnvironmentStrings()" );
        LPTSTR lpszVariable = (LPTSTR) lpvEnv;

        // Variable strings are separated by NULL byte, and the block is
        // terminated by a NULL byte.

        while (*lpszVariable)
        {
            //_tprintf(TEXT("%s\n"), lpszVariable);
            std::cerr << lpszVariable << std::endl;
            lpszVariable += lstrlen(lpszVariable) + 1;
        }

        ::FreeEnvironmentStrings(lpvEnv);
#endif
    }

    void environment:: get(glossary<string,string> &dict)
    {
        dict.free();

    }


}


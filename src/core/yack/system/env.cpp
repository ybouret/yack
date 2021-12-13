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

#include <cstring>

namespace yack
{

    namespace
    {
#if defined(YACK_WIN)
        class ESQuery
        {
        public:
            inline ESQuery() : lpvEnv(::GetEnvironmentStrings())
            {
				if(!lpvEnv) throw win32::exception(::GetLastError(), "GetEnvironmentStrings()");
            }

			inline LPTSTR operator*() throw()
			{
				return (LPSTR)lpvEnv;
			}

            inline ~ESQuery() throw()
            {
				::FreeEnvironmentStrings(lpvEnv);
				lpvEnv = 0;
            }

			LPTCH lpvEnv;

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
            std::cerr << *env << std::endl;
        }
#endif

#if defined(YACK_WIN)
		ESQuery esq;
		LPTSTR lpszVariable = *esq;

        // Variable strings are separated by NULL byte, and the block is
        // terminated by a NULL byte.

        while (*lpszVariable)
        {
            //_tprintf(TEXT("%s\n"), lpszVariable);
            std::cerr << lpszVariable << std::endl;
            lpszVariable += lstrlen(lpszVariable) + 1;
        }

       
#endif
    }

	static inline void grow(glossary<string, string> &dict, const string &es)
	{
        const char * const entry = es();
        const char *       equal = strchr(entry,'=');
        const char * const leave = entry + es.size();

        std::cerr << "[" << es << "]" << std::endl;

        if(!equal) throw exception("bad environment string");
        const string key(entry,equal-entry); ++equal;
        const string val(equal,leave-equal);
        std::cerr << "\t|_key='" << key << "'" << std::endl;
        std::cerr << "\t|_val='" << val << "'" << std::endl;
        if(!dict.insert(key,val))
        {
            throw exception("unexpected multiple ENV '%s'",key());
        }
	}

    void environment:: get(glossary<string,string> &dict)
    {
        dict.free();
        YACK_GIANT_LOCK();
#if defined(YACK_WIN)
		ESQuery esq;
		LPTSTR  lpszVariable = *esq;
		while (*lpszVariable)
		{
			//_tprintf(TEXT("%s\n"), lpszVariable);
			size_t sz = lstrlen(lpszVariable);
			const string es((const char *)lpszVariable, sz);
			grow(dict, es);
			lpszVariable += ++sz;
		}
#endif

#if defined(YACK_BSD)
        for(char **env=environ;*env;++env)
        {
            const string es(*env);
            grow(dict,es);
        }
#endif

    }


}


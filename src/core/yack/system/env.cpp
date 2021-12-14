#include "yack/system/env.hpp"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"
#include <iostream>

#if defined(YACK_BSD)
#include <unistd.h>
#include <cerrno>
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

    static inline void check(const char *equal)
    {
        if (!equal) throw exception("bad %s environment string", YACK_PLATFORM);
    }

    static inline void grow(glossary<string, string> &dict, const string &es)
    {
        const char * const entry = es();
		const char *       equal = strchr(entry+1, '='); if (!equal) return;
        const char * const leave = entry + es.size();
        const string       key(entry, equal - entry); ++equal;
        const string       val(equal, leave - equal);

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
            size_t       sz = lstrlen(lpszVariable);
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


    bool environment:: get(const string &key, string *val)
    {
        YACK_GIANT_LOCK();
#if defined(YACK_BSD)
        const char *res = getenv(key());
        if(!res)
        {
            if(val) val->clear();
            return false;
        }
        else
        {
            if(val) *val = res;
            return true;
        }
#endif

#if defined(YACK_WIN)
        if (val) val->clear();
        ESQuery esq;
        LPTSTR  lpszVariable = *esq;
        while (*lpszVariable)
        {
            //_tprintf(TEXT("%s\n"), lpszVariable);
            size_t sz = lstrlen(lpszVariable);
            const string es((const char *)lpszVariable, sz);
            const char * const entry = es();
            const char *       equal = strchr(entry+1, '='); 
			if (equal)
			{
				const string       sub(entry, equal - entry);
				if (sub == key)
				{
					if (val)
					{
						++equal;
						*val = string(equal, (entry + es.size()) - equal);
					}
					return true;
				}
			}
            lpszVariable += ++sz;
        }
        return false;
#endif
    }

    bool environment:: get(const char *key, string *val)
    {
        const string _(key);
        return get(_,val);
    }


    void environment:: set(const string &key, const string &val)
    {
        YACK_GIANT_LOCK();

#if defined(YACK_BSD)
        if(0!=(setenv(key(),val(),1)))
        {
            throw libc::exception(errno,"setenv(%s,%s)",key(),val());
        }
#endif

#if defined(YACK_WIN)
        if(!::SetEnvironmentVariable(key(),val()))
        {
            throw win32::exception(::GetLastError(),"SetEnv(%s,%s)",key(),val());
        }
#endif

    }


    void environment:: set(const string &key, const char *val)
    {
        const string __(val);
        set(key,__);
    }

    void environment:: set(const char   *key, const string &val)
    {
        const string _(key); set(_,val);
    }

    void environment:: set(const char   *key, const char *val)
    {
        const string _(key);
        const string __(val); set(_,__);
    }

    void    environment:: clr(const string &key)
    {
        YACK_GIANT_LOCK();

#if defined(YACK_BSD)
        if( 0 != unsetenv(key())) throw libc::exception(errno,"unsetenv(%s)",key());
#endif

#if defined(YACK_WIN)
        if(! ::SetEnvironmentVariable(key(),NULL) ) throw win32::exception(::GetLastError(),"unsetenv(%s)",key());
#endif

    }

    void    environment:: clr(const char *key)
    {
        const string _(key);
        clr(_);
    }
}


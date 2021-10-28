#include "yack/rtld/dll.hpp"
#include "yack/system/exception.hpp"

#if defined(YACK_BSD)
#include <dlfcn.h>
#endif

#if defined(YACK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h?
#endif

namespace yack
{
    dll:: ~dll() throw()
    {
        assert(handle);
#if     defined(YACK_BSD)
        (void) dlclose(handle);
#endif

#if     defined(YACK_WIN)
        (void) ::FreeLibrary(handle);
#endif

        handle = 0;
    }

#if defined(YACK_BSD)
    static inline void *dso_open(const char *soname)
    {
        assert(soname);
        void *h = dlopen(soname,RTLD_NOW|RTLD_LOCAL);
        if(!h)
        {
            imported::exception excp;
            excp.describe("dlopen(%s)",soname);
            excp.add("%s",dlerror());
            throw excp;
        }
        return h;
    }
#endif

#if defined(YACK_WIN)
    static inline void *dso_open(const char *soname)
    {
        assert(soname);
        HMODULE *h = ::LoadLibrary( soname );
        if(!h)
        {
            throw win32::exception(::GetLastError,"LoadLibrary(%s)",soname);
        }
        return h;
    }
#endif


    dll:: dll(const char *soname) :
    handle(dso_open(soname))
    {

    }

    void    * dll:: load(const char *symbol) const throw()
    {
        assert(symbol);
#if     defined(YACK_BSD)
        return dlsym((void*)handle,symbol);
#endif
#if     defined(YACK_WIN)
        return ::GetProcAddress((HMODULE)handle,symbol);
#endif

        return NULL;
    }


}

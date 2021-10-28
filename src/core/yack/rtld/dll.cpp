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

#if defined(YACK_WIN)
        (void) ::FreeLibrary(handle);
#endif

        handle = 0;
    }

#if defined(YACK_BSD)
    static inline void *load_so(const char *soname)
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
    static inline void *load_so(const char *soname)
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
    handle(load_so(soname))
    {

    }



}

#include "yack/fs/local/fs.hpp"
#include "yack/string.hpp"
#include "yack/system/exception.hpp"

#if defined(YACK_BSD)
#include <unistd.h>
#include <cerrno>
#include <dirent.h>
#endif

#if defined(YACK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


namespace yack
{

    const char local_fs::call_sign[] = "local_fs";

    local_fs:: local_fs() throw() : vfs(), singleton<local_fs>()
    {
    }

    local_fs:: ~local_fs() throw()
    {
    }


    void local_fs:: remove_file(const string &path)
    {
        YACK_GIANT_LOCK();
#if defined(YACK_BSD)
        if( unlink(path()) != 0)
        {
            throw libc::exception(errno,"unlink(%s)",path());
        }
#endif

#if defined(YACK_WIN)
        if( ! ::DeleteFile(path()) )
        {
            throw win32::exception( ::GetLastError(), "DeleteFile(%s)", path() );
        }
#endif

    }

}


namespace yack
{

    namespace
    {
        class local_scanner : public vfs::scanner
        {
        public:

#if defined(YACK_WIN)

#endif

#if defined(YACK_BSD)
            inline virtual ~local_scanner() throw()
            {
                closedir(hdir);
                hdir = 0;
            }
            
            inline local_scanner(const string &dirname) :
            vfs::scanner(dirname),
            hdir( setup(path) )
            {
            }

            static inline DIR *setup( const string &dirname )
            {
                YACK_GIANT_LOCK();
                DIR *h = opendir( dirname() );
                if(!h) throw libc::exception(errno,"opendir(%s)",dirname());
                return h;
            }

            inline virtual vfs::entry *next()
            {
                YACK_GIANT_LOCK();
                errno = 0;
                const dirent *ep = readdir(hdir);
                if(!ep)
                {
                    if(0!=errno) throw libc::exception(errno,"readdir(%s)",path());
                    return NULL;
                }
                else
                {
                    return new vfs::entry(ep->d_name);
                }
            }

            DIR *hdir;
#endif
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(local_scanner);
        };
    }

    vfs::scanner * local_fs:: open_folder(const string &path)
    {
        return new local_scanner(path);
    }

}

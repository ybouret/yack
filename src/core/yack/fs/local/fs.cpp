#include "yack/fs/local/fs.hpp"
#include "yack/string.hpp"
#include "yack/system/exception.hpp"
#include <cstring>

#if defined(YACK_BSD)
#include <unistd.h>
#include <cerrno>
#include <dirent.h>
#include <sys/stat.h>
#endif

#if defined(YACK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "yack/ptr/auto.hpp"
#endif


namespace yack
{

    const char local_fs::call_sign[] = "localFS";

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
        if( ! DeleteFile(path()) )
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
            inline virtual ~local_scanner() throw()
            {
                FindClose(hfind);
            }

            inline explicit local_scanner(const vfs &fs,
				const string &dirname) :
            vfs::scanner(fs,dirname),
            hfind(INVALID_HANDLE_VALUE),
            has(false),
            wfd()
            {
                YACK_GIANT_LOCK();
                if (dirname.size() > MAX_PATH - 3) throw exception("FindFirstFile(dirname is too long)");
                string szDir = dirname + "\\*";
                hfind = FindFirstFile(szDir(), &wfd);
                if (INVALID_HANDLE_VALUE == hfind)
                {
                    throw win32::exception(GetLastError(), "FindFirstFile(%s)", dirname());
                }
                has = true;
            }

            virtual vfs::entry *next()
            {
                if(has)
                {
                    const string         here = path + wfd.cFileName;
                    auto_ptr<vfs::entry> ep   = new vfs::entry(root,here);
                    has = false;
                    YACK_GIANT_LOCK();
                    if(!FindNextFile(hfind,&wfd))
                    {
                        const DWORD err = GetLastError();
                        if (err != ERROR_NO_MORE_FILES) throw win32::exception(err, "FindNextFile(%s)", path());
                    }
                    else
                    {
                        has = true;
                    }
                    return ep.yield();
                }
                else
                {
                    return NULL;
                }
            }
            HANDLE               hfind;
            bool                 has;
            WIN32_FIND_DATA      wfd;
#endif

#if defined(YACK_BSD)
            inline virtual ~local_scanner() throw()
            {
                closedir(hdir);
                hdir = 0;
            }
            
            inline local_scanner(const vfs    &fs,
                                 const string &dirname) :
            vfs::scanner(fs,dirname),
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
                    const string   here = path + ep->d_name;
                    return new vfs::entry(root,here);
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
        return new local_scanner(*this,path);
    }

    vfs::entry::attr_t local_fs:: get_attr_of(const string &path) const
    {
        YACK_GIANT_LOCK();
        entry::attr_t attr = entry::attr_unk;

#if defined(YACK_BSD)
        struct stat buf;
        memset(&buf,0,sizeof(buf));
        if( 0 != lstat(path(),&buf) ) throw libc::exception(errno,"lstat(%s)",path());
        const mode_t m = (buf.st_mode & S_IFMT);
        std::cerr << "mode=" << m << " for '" << path << "'" << std::endl;
        if( m & S_IFREG ) attr |= entry::attr_reg;
        if( m & S_IFDIR ) attr |= entry::attr_dir;
        if( m & S_IFLNK ) attr |= entry::attr_lnk;
#endif

#if defined(YACK_WIN)

#endif

        return attr;
    }


}

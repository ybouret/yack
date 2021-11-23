
#include "yack/fs/vfs.hpp"
#include "yack/type/cstring.h"
#include "yack/string.hpp"
#include <cstring>

namespace yack
{

    const char *vfs:: get_base_name(const char *path, const size_t plen) throw()
    {
        if(plen<=0)
        {
            return path;
        }
        else
        {
            const char *scan = path+plen;
            while(scan>path)
            {
                switch( *(--scan) )
                {
                    case '\\':
                    case '/':
                        return ++scan;
                    default:
                        break;
                }
            }
            assert(scan==path);
            return path;
        }
    }

    const char *vfs:: get_base_name(const string &path) throw()
    {
        return get_base_name(&path[1],path.size());
    }


    const char *vfs:: get_base_name(const char *path) throw()
    {
        return get_base_name(path,yack_cstring_size(path));
    }

}

namespace yack
{

    const char *vfs:: get_extension(const char *path, const size_t plen) throw()
    {
        if(plen<=0)
        {
            return NULL;
        }
        else
        {
            const char *scan = path+plen;
            while(scan>path)
            {
                switch( *(--scan) )
                {
                    case '.':
                        return ++scan;
                    default:
                        break;
                }
            }
            assert(scan==path);
            return NULL;
        }
    }


    const char *vfs:: get_extension(const char *path) throw()
    {
        return get_extension(path,yack_cstring_size(path));

    }

    const char *vfs:: get_extension(const string &path) throw()
    {
        return get_extension(&path[1],path.size());
    }

}


namespace yack
{

    string vfs:: make_null_ext(const char   *path, const size_t plen)
    {
        const char *ext = get_extension(path,plen);
        if(ext)
        {
            return string(path,--ext-path);
        }
        else
        {
            return string(path,plen);
        }
    }

    string vfs:: make_null_ext(const char   *path)
    {
        if(path)
        {
            return make_null_ext(path,strlen(path));
        }
        else
        {
            return string();
        }
    }

    string vfs:: make_null_ext(const string &path)
    {
        return make_null_ext(&path[1],path.size());
    }

}

namespace yack
{

    string vfs::new_extension(const char *path, const size_t plen, const char *ext)
    {
        string raw = make_null_ext(path,plen);
        if(ext)
        {
            raw += '.';
            raw += ext;
            return raw;
        }
        else
        {
            return raw;
        }
    }

    string vfs:: new_extension(const char *path, const char *ext)
    {
        return new_extension(path,path?strlen(path):0,ext);
    }

    string vfs:: new_extension(const string &path, const char *ext)
    {
        return new_extension(&path[1],path.size(),ext);
    }

    string vfs::new_extension(const string &path, const string &ext)
    {
        return new_extension(&path[1],path.size(),&ext[1]);
    }


}

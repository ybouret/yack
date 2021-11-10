
#include "yack/fs/vfs.hpp"
#include "yack/type/cstring.h"
#include "yack/memory/allocator/legacy.hpp"

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

    const char *vfs:: get_base_name(const char *path) throw()
    {
        return get_base_name(path,yack_cstring_size(path));
    }

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


    


}



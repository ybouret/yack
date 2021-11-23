
#include "yack/ios/c/readable.hpp"
#include "yack/lockable.hpp"
#include "yack/system/exception.hpp"
#include <cstdio>
#include <cerrno>

namespace yack
{
    namespace ios
    {
        readable_file:: ~readable_file() throw()
        {
        }


        readable_file:: readable_file(const cstdin_t &_) :
        c_file(_)
        {
        }

        readable_file:: readable_file(const char *filename) :
        c_file(filename,r)
        {
        }

        readable_file:: readable_file(const string &filename) :
        c_file(filename,r)
        {
        }
        
        bool readable_file:: get(char &C)
        {
            YACK_GIANT_LOCK();
            FILE        *fp = static_cast<FILE *>(handle);
            const size_t nr = fread(&C,1,1,fp);
            if(nr<=0)
            {
                if(feof(fp)) return false;
                throw libc::exception(errno,"ios::readable_file::get(char)");
            }
            else
            {
                return true;
            }
        }

        size_t readable_file:: get(void *addr, const size_t size)
        {
            assert(addr!=NULL);
            assert(size>0);
            FILE        *fp = static_cast<FILE *>(handle);
            const size_t nr = fread(addr,1,size,fp);
            if(nr<size && ferror(fp))
                throw libc::exception(errno,"ios::readable_file::get(%lu)",(unsigned long)size);
            return nr;
        }

    }

}


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
        
        bool readable_file:: get(char &C)
        {
            YACK_GIANT_LOCK();
            FILE        *fp = static_cast<FILE *>(handle);
            const size_t nr = fread(&C,1,1,fp);
            if(nr<=0)
            {
                if(feof(fp)) return false;
                throw libc::exception(errno,"ios::readable_file::get");
            }
            else
            {
                return true;
            }
        }

    }

}

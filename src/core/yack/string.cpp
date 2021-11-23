
#include "yack/string.hpp"
#include "yack/system/exception.hpp"
#include "yack/lockable.hpp"

#include <cstdarg>
#include <cerrno>

namespace yack
{
    string vformat(const char *fmt,...)
    {
        static const char fn[] = "vsnprintf";

        assert(fmt!=NULL);
        YACK_GIANT_LOCK();
        int res = 0;
        {
            va_list ap;
            va_start(ap,fmt);
            res = vsnprintf(NULL,0,fmt,ap);
            va_end(ap);
            if(res<0) throw libc::exception(errno,"%s(...)",fn);
        }
        if(res<=0)
        {
            return string();
        }
        else
        {
            string  ans(res,as_capacity,true); assert( ans.size() == size_t(res) );
            va_list ap;
            va_start(ap,fmt);
            const int chk = vsnprintf(&ans[1],ans.size()+1,fmt,ap);
            va_end(ap);
            if(res!=chk) throw  exception("%s mismatch!",fn);
            return ans;
        }
    }
}

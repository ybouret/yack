#include "yack/net/types.hpp"
#include "yack/system/error.hpp"
#include <cstring>
#include <cstdarg>

namespace yack {

    namespace net
    {
        exception:: ~exception() throw()
        {
            memset(what_,0,sizeof(what_));
        }

        exception:: exception(const exception &other) throw() :
        yack::exception(other),
        code_(other.code_),
        what_()
        {
            memcpy(what_,other.what_,sizeof(what_));
        }

        exception:: exception(const error_code err, const char *fmt, ...) throw() :
        yack::exception(),
        code_(err),
        what_()
        {
            assert(fmt);
            {
                va_list ap;
                va_start(ap, fmt);
                failsafe_format(info, sizeof(info), fmt, &ap);
                va_end(ap);
            }

            YACK_SYSTEM_ERROR_FORMAT(what_,sizeof(what_),code_);
        }

        const char * exception:: what() const throw()
        {
            return what_;
        }
    }

}

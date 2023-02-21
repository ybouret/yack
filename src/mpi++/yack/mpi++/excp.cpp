#include "yack/mpi++/mpi.hpp"
#include <cstring>
#include <cstdarg>

namespace yack
{
    mpi:: exception:: ~exception() noexcept
    {
        memset(data,0,sizeof(data));
    }

    const char * mpi::exception::what() const noexcept
    {
        return data;
    }

    mpi:: exception:: exception(const int err,
                                const char *fmt,
                                ...) noexcept :
    yack::exception(),
    code(err),
    data()
    {
        memset(data,0,sizeof(data));

        // format when
        assert(fmt);
        va_list ap;
        va_start(ap,fmt);
        failsafe_format(info,sizeof(info),fmt,&ap);
        va_end(ap);

        // format what
        int resultlen = 0;
        (void) MPI_Error_string(code,data,&resultlen);
    }

    mpi::exception::exception(const exception &other) noexcept :
    yack::exception(other),
    code(other.code),
    data()
    {
        memcpy(data, other.data, sizeof(data));
    }

}

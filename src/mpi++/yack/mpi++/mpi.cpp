#include "yack/mpi++/mpi.hpp"
#include <cstring>

namespace yack
{
    mpi:: exception:: ~exception() throw()
    {
        memset(data,0,sizeof(data));
    }

    const char * mpi::exception::what() const throw()
    {
        return data;
    }

    mpi:: exception:: exception(const int err,
                                const char *fmt,
                                ...) throw() :
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

}

namespace yack {


    const char * const mpi::call_sign = "mpi";

    mpi:: ~mpi() throw()
    {
    }


    mpi:: mpi() : singleton<mpi>()
    {
    }


}

#include "yack/mpi++/mpi.hpp"
#include <cstring>
#include <cstdarg>

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

#include "yack/type/temporary.hpp"

namespace yack {


    const char * const mpi::call_sign = "mpi";

    mpi:: ~mpi() throw()
    {
        (void) MPI_Finalize();
    }


    namespace {
        static int *    __mpi_argc = NULL;
        static char *** __mpi_argv = NULL;
    }

    mpi & mpi:: Init(int    &argc,
                     char **&argv)
    {
        if(NULL==argv) throw yack::exception("%s::Init(%d,NULL)",call_sign,argc);
        const temporary<int*>    keep_argc(__mpi_argc,&argc);
        const temporary<char***> keep_argv(__mpi_argv,&argv);
        return mpi::instance();
    }

    mpi:: mpi() : singleton<mpi>(), rank(0), size(0), threading(0)
    {
        if(NULL==__mpi_argc || NULL==__mpi_argv) throw yack::exception("%s: need to call mpi::Init()",call_sign);

        int  required = MPI_THREAD_MULTIPLE;
        int *provided = &coerce(threading);
        YACK_MPI_CALL(MPI_Init_thread(__mpi_argc,__mpi_argv,required,provided));
        try
        {
            YACK_MPI_CALL( MPI_Comm_rank(MPI_COMM_WORLD,& coerce(rank) ) );
            YACK_MPI_CALL( MPI_Comm_size(MPI_COMM_WORLD,& coerce(size) ) );
        }
        catch(...)
        {
            MPI_Finalize();
            throw;
        }
    }


}

#include "yack/mpi++/mpi.hpp"
#include "yack/type/temporary.hpp"
#include "yack/associative/be-addr.hpp"

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

    mpi:: mpi() : singleton<mpi>(), rank(0), size(0), threading(0), dtdb()
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

    MPI_Datatype mpi:: DataType(const rtti &tid) const
    {
        return dtdb(tid);
    }

}

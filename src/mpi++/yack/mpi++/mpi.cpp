#include "yack/mpi++/mpi.hpp"
#include "yack/type/temporary.hpp"
#include "yack/associative/be-addr.hpp"
#include "yack/arith/base10.hpp"
#include <cstring>

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

    namespace {
        static inline void mpi_build_name(string &name, const int size, const int rank )
        {
            assert(size>0);
            assert(rank>=0);
            assert(rank<size);
            const unsigned digits = digits_for(size);
            const string   fmt    = vformat("%%%ud.%%0%ud",digits,digits);
            name = vformat(fmt(),size,rank);
        }
    }


    void mpi:: tmx_init() throw()
    {
        memset( & coerce(send_tmx), 0, sizeof(send_tmx) );
        memset( & coerce(recv_tmx), 0, sizeof(recv_tmx) );
    }

    mpi:: mpi() :
    singleton<mpi>(),
    rank(0),
    size(0),
    primary(false),
    replica(false),
    threading(0),
    name(),
    send_tmx(),
    recv_tmx(),
    native()
    {
        if(NULL==__mpi_argc || NULL==__mpi_argv) throw yack::exception("%s: need to call mpi::Init()",call_sign);

        // make initial call
        int  required = MPI_THREAD_MULTIPLE;
        int *provided = &coerce(threading);
        YACK_MPI_CALL(MPI_Init_thread(__mpi_argc,__mpi_argv,required,provided));

        // post-process
        try
        {
            // size/rank
            YACK_MPI_CALL( MPI_Comm_rank(MPI_COMM_WORLD,& coerce(rank) ) );
            YACK_MPI_CALL( MPI_Comm_size(MPI_COMM_WORLD,& coerce(size) ) );

            // find role
            if(0==rank)
            {
                coerce(primary) = true;
            }
            else
            {
                coerce(replica) = true;
            }

            // build name
            mpi_build_name(coerce(name), size, rank);

            tmx_init();

        }
        catch(...)
        {
            MPI_Finalize();
            throw;
        }
    }

    const __mpi::data_type &mpi:: DataType(const rtti &tid) const
    {
        return native(tid);
    }

}

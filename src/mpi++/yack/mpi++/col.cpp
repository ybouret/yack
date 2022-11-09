
#include "yack/mpi++/mpi.hpp"
#include "yack/string.hpp"
#include <cstring>
#include <cstdarg>

namespace yack
{

    void mpi:: Bcast(void              *buf,
                     const size_t       num,
                     const MPI_Datatype tid,
                     const unsigned     bpi,
                     const int          root) const
    {
        static const size_t nmax( integral_for<int>::maximum );
        if(num>nmax) throw exception(MPI_ERR_COUNT,"MPI_Bcast overflow");
        const uint64_t tmark = wtime::ticks();
        YACK_MPI_CALL(MPI_Bcast(buf,int(num),tid,root,MPI_COMM_WORLD));
        const uint64_t ticks = wtime::ticks() - tmark;
        const uint64_t bytes = bpi*num;
        tmx           &state = (root==rank) ? coerce(send_tmx) : coerce(recv_tmx);
        state.bytes += bytes;
        state.ticks += ticks;
    }

}


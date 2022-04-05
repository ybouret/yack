#include "yack/mpi++/mpi.hpp"
#include <cstring>
#include <cstdarg>

namespace yack
{

    void mpi:: Send(const void        *buf,
                    const size_t       num,
                    const MPI_Datatype tid,
                    const int          dst,
                    const int          tag) const
    {
        YACK_MPI_CALL(MPI_Send(buf,num,tid,dst,tag,MPI_COMM_WORLD));
    }

    static const uint8_t SYN_ACK = 0xff;

    void mpi:: SYN(const int dst, const int tag) const
    {
        const uint8_t msg = SYN_ACK;
        Send(&msg,1,MPI_BYTE,dst,tag);
    }

    void mpi:: Recv(void *buf, const size_t num, const MPI_Datatype tid, const int src, const int tag) const
    {
        MPI_Status status;
        YACK_MPI_CALL(MPI_Recv(buf,num,tid,src,tag,MPI_COMM_WORLD,&status));
    }

    void mpi:: ACK(const int src, const int tag) const
    {
        uint8_t msg = 0x00;
        Recv(&msg,1,MPI_BYTE,src,tag);
        if(SYN_ACK!=msg) throw yack::exception("%s: invalid SYN/ACK!!",call_sign);
    }


}



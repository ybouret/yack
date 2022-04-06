#include "yack/mpi++/mpi.hpp"
#include "yack/string.hpp"
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


    void mpi:: SYN(const int dst) const
    {
        static const uint8_t msg = syn_ack;
        Send(&msg,1,MPI_BYTE,dst,io_tag);
    }

    void mpi:: primary_sync() const
    {
        assert(primary);
        for(int r=1;r<size;++r)
        {
            SYN(r);
            ACK(r);
        }
    }

    void mpi:: replica_wait() const
    {
        assert(replica);
        ACK(0);
    }

    void mpi:: replica_done() const
    {
        assert(replica);
        SYN(0);
    }

    void mpi:: Recv(void *buf, const size_t num, const MPI_Datatype tid, const int src, const int tag) const
    {
        MPI_Status status;
        YACK_MPI_CALL(MPI_Recv(buf,num,tid,src,tag,MPI_COMM_WORLD,&status));
    }

    void mpi:: ACK(const int src) const
    {
        uint8_t msg = 0x00;
        Recv(&msg,1,MPI_BYTE,src,io_tag);
        if(syn_ack!=msg) throw yack::exception("%s: invalid SYN/ACK!!",call_sign);
    }

    template <>
    void mpi:: Send<string>(const string &obj, const int dst, const int tag) const
    {
        const size_t len = obj.size();
        Send(len,dst,tag);
        if(len>0)
        {
            Send(obj(),len,dst,tag);
        }
    }

    template <>
    string mpi:: Recv<string>(const int src, const int tag) const
    {
        const size_t len = Recv<size_t>(src,tag);
        if(len>0)
        {
            string s(len,as_capacity,true);
            Recv(s(),len,src,tag);
            return s;
        }
        else
        {
            return string();
        }
    }

}



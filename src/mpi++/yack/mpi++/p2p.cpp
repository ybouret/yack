#include "yack/mpi++/mpi.hpp"
#include "yack/string.hpp"
#include <cstring>
#include <cstdarg>

namespace yack
{

    void mpi:: Send(const void        *buf,
                    const size_t       num,
                    const MPI_Datatype tid,
                    const unsigned     bpi,
                    const int          dst,
                    const int          tag) const
    {
        YACK_MPI_TMX_INIT();
        YACK_MPI_CALL(MPI_Send(buf,num,tid,dst,tag,MPI_COMM_WORLD));
        YACK_MPI_TMX_DONE(send,bpi*num);
    }


    void mpi:: SYN(const int dst) const
    {
        static const uint8_t msg = syn_ack;
        YACK_MPI_TMX_INIT();
        Send(&msg,1,MPI_BYTE,1,dst,io_tag);
        YACK_MPI_TMX_DONE(send,1);
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

    void mpi:: Recv(void              *buf,
                    const size_t       num,
                    const MPI_Datatype tid,
                    const unsigned     bpi,
                    const int          src,
                    const int          tag) const
    {
        YACK_MPI_TMX_INIT();
        {
            MPI_Status status;
            YACK_MPI_CALL(MPI_Recv(buf,num,tid,src,tag,MPI_COMM_WORLD,&status));
        }
        YACK_MPI_TMX_DONE(recv,bpi*num);
    }

    void mpi:: ACK(const int src) const
    {
        uint8_t msg = 0x00;
        {
            YACK_MPI_TMX_INIT();
            Recv(&msg,1,MPI_BYTE,1,src,io_tag);
            YACK_MPI_TMX_DONE(recv,1);
        }
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



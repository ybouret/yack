#include "yack/mpi++/mpi.hpp"
#include "yack/type/ints.hpp"

namespace yack
{

    void mpi:: Send(const void        *buf,
                    const size_t       num,
                    const MPI_Datatype tid,
                    const unsigned     bpi,
                    const int          dst,
                    const int          tag) const
    {
        static const size_t nmax( integral_for<int>::maximum );
        if(num>nmax) throw  exception(MPI_ERR_COUNT,"MPI_Send overflow");
        YACK_MPI_TMX_INIT();
        YACK_MPI_CALL(MPI_Send(buf,int(num),tid,dst,tag,MPI_COMM_WORLD));
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
        static const size_t nmax = size_t( integral_for<int>::maximum );
        if(num>nmax) throw  exception(MPI_ERR_COUNT,"MPI_Recv overflow");
        YACK_MPI_TMX_INIT();
        {
            MPI_Status status;
            YACK_MPI_CALL(MPI_Recv(buf,int(num),tid,src,tag,MPI_COMM_WORLD,&status));
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

    
}





#include "yack/mpi++/data/srz.hxx"
#include "yack/mpi++/mpi.hpp"
#include "yack/system/exception.hpp"

namespace yack
{
    namespace __mpi
    {

        srz_io:: srz_io(const rtti &uuid) :
        data_io(uuid),
        io(4096)
        { }

        srz_io:: ~srz_io() throw()
        {
        }

        void srz_io:: send1(const mpi               &MPI,
                            const ios::serializable &srz,
                            const int                dst,
                            const int                tag) const
        {
            static const data_type mdt = MPI.DataType<size_t>();
            io->free();
            const size_t n = srz.serialize(io);
            MPI.Send(&n,1,mdt.info,mdt.size,dst,tag);
            if(n>0)
            {
                assert(n==io->size());
                MPI.Send(io.buffer(),n,MPI_BYTE,1,dst,tag);
            }
        }

        size_t srz_io:: recv_size(const mpi &MPI, const int src, const int tag)
        {
            static const data_type mdt = MPI.DataType<size_t>();
            size_t res = 0;
            MPI.Recv(&res,1,mdt.info,mdt.size,src,tag);
            return res;
        }

        void srz_io::recv_buff(const mpi &MPI, const size_t len, const int src, const int tag) const
        {
            assert(len>0);
            io->adjust(len,0);
            MPI.Recv(io.buffer(),len,MPI_BYTE,1,src,tag);
        }

        void srz_io:: check_len(const size_t len, const size_t cm) const
        {
            if(len != cm)
            {
                const string & name = tid.name();
                throw libc::exception(EIO,"recv<%s> length mismatch",name());
            }
        }


    }

}


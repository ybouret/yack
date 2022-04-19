#include "yack/mpi++/data/lcplx.hxx"
#include "yack/mpi++/mpi.hpp"

namespace yack
{
    namespace __mpi
    {

        lcplx_io:: lcplx_io(const data_types &native) :
        data_io( rtti::use<complex_type>() ),
        mdt(    native.get<native_type>() )
        {
        }

        void lcplx_io :: send(const mpi   &MPI,
                              const void  *ptr,
                              const size_t num,
                              const int    dst,
                              const int    tag) const
        {
            MPI.Send(ptr,2*num,mdt.info,mdt.size,dst,tag);
        }

        void lcplx_io ::  recv(const mpi   &MPI,
                               void        *ptr,
                               const size_t num,
                               const int    src,
                               const int    tag) const
        {
            MPI.Recv(ptr,2*num,mdt.info,mdt.size,src,tag);
        }

        lcplx_io:: ~lcplx_io() throw() {}

    }

}


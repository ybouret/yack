
#ifndef YACK_MPICXX_DATA_LCPLX_INCLUDED
#define YACK_MPICXX_DATA_LCPLX_INCLUDED 1

#include "yack/mpi++/data/io.hpp"
#include "yack/mpi++/data/types.hpp"
#include "yack/type/complex.hpp"

namespace yack
{
    class mpi;

    namespace __mpi
    {
        class lcplx_io : public data_io
        {
        public:
            typedef long double           native_type;
            typedef complex<native_type>  complex_type;

            explicit lcplx_io(const data_types &native);
            virtual ~lcplx_io() throw();
            
            virtual void send(const mpi   &MPI,
                              const void  *ptr,
                              const size_t num,
                              const int    dst,
                              const int    tag) const;

            virtual void recv(const mpi   &MPI,
                              void        *ptr,
                              const size_t num,
                              const int    src,
                              const int    tag) const;

            const data_type mdt;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lcplx_io);
        };
        

    }
}
#endif


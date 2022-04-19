
#ifndef YACK_MPICXX_DATA_STRING_INCLUDED
#define YACK_MPICXX_DATA_STRING_INCLUDED 1

#include "yack/mpi++/data/io.hpp"
#include "yack/mpi++/data/types.hpp"

namespace yack
{
    class mpi;

    namespace __mpi
    {
        class string_io : public data_io_for<string>
        {
        public:

            explicit string_io();
            virtual ~string_io() throw();

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


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(string_io);

            void send1(const mpi    &MPI,
                       const string &str,
                       const int     dst,
                       const int     tag) const;

            void recv1(const mpi    &MPI,
                       string       &str,
                       const int     src,
                       const int     tag) const;

        };

    }

}

#endif

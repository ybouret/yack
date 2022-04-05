//! \file

#ifndef YACK_MPICXX_INCLUDED
#define YACK_MPICXX_INCLUDED 1

#include "yack/singleton.hpp"
#include "yack/exception.hpp"
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>

namespace yack
{

    class mpi : public singleton<mpi>
    {
    public:
        static const at_exit::longevity life_time = 2000;
        static const char  * const      call_sign;

        class exception : public yack::exception
        {
        public:
            virtual ~exception() throw();
            explicit exception(const int err,const char *fmt,...) throw() YACK_PRINTF_CHECK(3,4);
            virtual  const char *what() const throw();

            const int code;
        private:
            YACK_DISABLE_ASSIGN(exception);
            char data[MPI_MAX_ERROR_STRING];
        };


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(mpi);
        friend class singleton<mpi>;
        explicit mpi();
        virtual ~mpi() throw();
    };


}


#endif


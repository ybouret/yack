//! \file

#ifndef YACK_MPICXX_INCLUDED
#define YACK_MPICXX_INCLUDED 1

#include "yack/singleton.hpp"
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>

namespace yack
{

    class mpi : public singleton<mpi>
    {
    public:
        static const at_exit::longevity life_time = 2000;
        static const char  * const      call_sign;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(mpi);
        friend class singleton<mpi>;
        explicit mpi();
        virtual ~mpi() throw();
    };


}


#endif


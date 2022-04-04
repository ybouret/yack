//! \file

#ifndef YACK_MPICXX_INCLUDED
#define YACK_MPICXX_INCLUDED 1

#include "yack/singleton.hpp"
#define OMPI_SKIP_MPICXX 1

namespace yack
{

    class mpi : public singleton<mpi>
    {
    public:
        

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(mpi);
    };


}


#endif


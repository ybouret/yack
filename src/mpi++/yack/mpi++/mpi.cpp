#include "yack/mpi++/mpi.hpp"

namespace yack {


    const char * const mpi::call_sign = "mpi";

    mpi:: ~mpi() throw()
    {
    }


    mpi:: mpi() : singleton<mpi>()
    {
    }


}

#include "yack/mpi++/mpi.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(init)
{
    mpi & MPI = mpi::instance();
    (void) MPI;
}
YACK_UDONE()



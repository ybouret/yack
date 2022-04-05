#include "yack/mpi++/mpi.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(init)
{
    mpi & MPI = mpi::Init(argc,argv);

    std::cerr << "from " << MPI.size << "." << MPI.rank << std::endl;

}
YACK_UDONE()



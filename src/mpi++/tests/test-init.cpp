#include "yack/mpi++/mpi.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace {

    static inline void greet(mpi &MPI)
    {
        std::cerr << "from " << MPI.size << "." << MPI.rank << std::endl;

    }
}

YACK_UTEST(init)
{
    mpi & MPI = mpi::Init(argc,argv);

    if(MPI.rank == 0 )
    {
        greet(MPI);
        for(int rank=1;rank<MPI.size;++rank)
        {
            MPI.SYN(rank);
            MPI.ACK(rank);
        }

        std::cerr << MPI.DataType<uint8_t>() << std::endl;
        std::cerr << MPI.DataType<char>()    << std::endl;

    }
    else
    {
        MPI.ACK(0);
        greet(MPI);
        MPI.SYN(0);
    }

}
YACK_UDONE()



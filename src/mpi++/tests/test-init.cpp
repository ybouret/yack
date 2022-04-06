#include "yack/mpi++/mpi.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

namespace {

    static inline void greet(mpi &MPI)
    {
        std::cerr << "from " << MPI.size << "." << MPI.rank << std::endl;
        std::cerr.flush();
    }
}

YACK_UTEST(init)
{
    mpi &  MPI = mpi::Init(argc,argv);
    string str;

    if(MPI.primary)
    {
        greet(MPI);
        MPI.primary_sync();

        str = "Hello, World!";
        
        for(int rank=1;rank<MPI.size;++rank)
        {
            MPI.Send(str,rank);
        }
    }
    else
    {
        assert(MPI.replica);
        MPI.replica_wait();
        greet(MPI);
        MPI.replica_done();

        str = MPI.Recv<string>(0);
    }

    if(MPI.primary)
    {
        std::cerr << "primary = " << str << std::endl;
        std::cerr.flush();
        MPI.primary_sync();
    }
    else
    {
        MPI.replica_wait();
        std::cerr << "replica = " << str << std::endl;
        std::cerr.flush();
        MPI.replica_done();
    }

}
YACK_UDONE()



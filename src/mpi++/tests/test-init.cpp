#include "yack/mpi++/mpi.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include <cstdio>

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


    }
    else
    {
        assert(MPI.replica);
        MPI.replica_wait();
        greet(MPI);
        MPI.replica_done();

    }

    MPI_Barrier(MPI_COMM_WORLD);


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

    MPI_Barrier(MPI_COMM_WORLD);

    YACK_SYNCHRONIZE(MPI,
                     std::cerr << "@" << MPI.name << std::endl;
                     std::cerr.flush());


    MPI.Printf(stderr, "From %s\n", MPI.name() );
}
YACK_UDONE()



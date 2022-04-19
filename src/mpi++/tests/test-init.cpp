#include "yack/mpi++/mpi.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/sequence/vector.hpp"
#include "../../core/tests/main.hpp"
#include "yack/data/bare.hpp"

#include <cstdio>

using namespace yack;

namespace {

    static inline void greet(mpi &MPI)
    {
        std::cerr << "from " << MPI.size << "." << MPI.rank << std::endl;
        std::cerr.flush();
    }

    template <typename T>
    static inline
    void testIO(const mpi &MPI,randomized::bits &ran)
    {
        const bare<T> zero;

        if(MPI.primary)
        {
            std::cerr << "test<" << rtti::name<T>() << ">" << std::endl;
            std::cerr.flush();
        }

        size_t N = 0;
        if(MPI.primary) N = 4 + ran.leq(6);
        MPI.Bcast(N,0);
        MPI.Printf(stderr, "\tN[%s]=%lu\n", MPI.name(), (unsigned long)N );

        vector<T> data(N,as_capacity);
        for(size_t i=N;i>0;--i)
        {
            if(MPI.primary)
            {
                const T tmp = bring::get<T>(ran);
                data << tmp;
            }
            else
            {
                data << *zero;
            }
        }

        if(MPI.primary)
        {
            for(int r=1;r<MPI.size;++r)
            {
                MPI.Send(&data[1],data.size(),r,7);
            }
        }
        else
        {
            MPI.Recv(&data[1],data.size(),0,7);
        }

        YACK_SYNCHRONIZE(MPI,
                         std::cerr << data << std::endl;
                         std::cerr.flush()
                         );

    }

}

YACK_UTEST(init)
{
    randomized::rand_ ran;

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



    testIO<int>(MPI,ran);
    testIO<float>(MPI,ran);
    testIO< complex<long double> >(MPI,ran);
    testIO< string >(MPI,ran);
    testIO< apn >(MPI,ran);
    testIO< apz >(MPI,ran);
    testIO< apq >(MPI,ran);

}
YACK_UDONE()



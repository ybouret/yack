
#include "yack/mpi++/mpi.hpp"
#include "yack/type/temporary.hpp"
#include "yack/associative/be-addr.hpp"
#include "yack/arith/base10.hpp"

namespace yack {

    unsigned mpi::data_size(const MPI_Datatype)
    {
        
        throw yack::exception("%s: unknown MPI_Datatype",call_sign);
    }

}

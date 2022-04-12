#include "yack/mpi++/data/type.hpp"

namespace yack
{
    namespace __mpi
    {

        data_type:: data_type(const MPI_Datatype t, const unsigned n) throw() :
        info(t),
        size(n)
        {
        }

        data_type:: ~data_type() throw()
        {
            coerce(info) = MPI_DATATYPE_NULL;
            coerce(size) = 0;
        }

        data_type:: data_type(const data_type &other) throw() :
        info(other.info),
        size(other.size)
        {
        }

    }

}


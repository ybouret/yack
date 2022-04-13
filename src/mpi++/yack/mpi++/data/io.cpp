

#include "yack/mpi++/data/io.hpp"


namespace yack
{

    namespace __mpi
    {
        data_io:: ~data_io() throw() {}

        data_io:: data_io(const rtti &t) throw() :
        object(),
        tid(t),
        bek(tid)
        {
        }

        const memory::ro_buffer & data_io:: key() const throw()
        {
            return bek;
        }
    }

}




#include "yack/mpi++/data/io.hpp"


namespace yack
{

    namespace __mpi
    {
        data_io:: ~data_io() throw() {}


        const memory::ro_buffer & data_io:: key() const throw()
        {
            return bek;
        }

        data_io:: data_io(const rtti &user_tid) throw() :
        object(),
        tid(user_tid),
        bek(tid)
        {
        }
    }

}

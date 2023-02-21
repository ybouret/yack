

#include "yack/mpi++/data/io.hpp"


namespace yack
{

    namespace __mpi
    {
        data_io:: ~data_io() noexcept {}


        const memory::ro_buffer & data_io:: key() const noexcept
        {
            return bek;
        }

        data_io:: data_io(const rtti &user_tid) noexcept :
        object(),
        tid(user_tid),
        bek(tid)
        {
        }
    }

}

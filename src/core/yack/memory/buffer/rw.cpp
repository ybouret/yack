

#include "yack/memory/buffer/rw.hpp"

namespace yack
{

    namespace memory
    {
        rw_buffer:: ~rw_buffer() noexcept
        {
        }

        rw_buffer:: rw_buffer() noexcept : ro_buffer()
        {
        }


        void * rw_buffer:: rw_addr() noexcept
        {
            return (void*)ro_addr();
        }


    }

}



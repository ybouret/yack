

#include "yack/memory/buffer/rw.hpp"

namespace yack
{

    namespace memory
    {
        rw_buffer:: ~rw_buffer() throw()
        {
        }

        rw_buffer:: rw_buffer() throw() : ro_buffer()
        {
        }


        void * rw_buffer:: rw_addr() throw()
        {
            return (void*)ro_addr();
        }


    }

}



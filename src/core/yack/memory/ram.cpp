
#include "yack/memory/ram.hpp"
#include "yack/system/error.h"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    static uint64_t ram_ = 0;

    uint64_t ram:: get() throw()
    {
        return ram_;
    }




    void * ram:: acquire(size_t &count,  const size_t block_size)
    {
        assert(0<block_size);
        if(count<=0)
        {
            return NULL;
        }
        else
        {
            void *addr = calloc(count,block_size);
            if(!addr)
            {
                const size_t n = count*block_size;
                count = 0;
                throw libc::exception(ENOMEM,"ram::acquire(%lu)",(unsigned long)n);
            }

            ram_  += ( count *= block_size );
            return addr;

        }

    }

    static inline
    void  check_more_ram_than(const size_t size)
    {
        if(size>ram_) yack_bsd_critical_error(EACCES," ram::release(more than allocated)");
    }

    void  ram::release(void * &addr, size_t &size) throw()
    {
        if(addr)
        {
            assert(size>0);
            check_more_ram_than(size);
            free(addr);
            ram_ -= size;
            addr = NULL;
            size = 0;
        }
        else
        {
            assert(size<=0);
        }
        
    }

}

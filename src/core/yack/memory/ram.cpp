
#include "yack/memory/ram.hpp"
#include "yack/system/error.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#include <iostream>

namespace yack
{
    namespace memory
    {
        
        uint64_t ram_ = 0;

        uint64_t ram:: allocated() throw()
        {
            return ram_;
        }

        ram:: ram() throw()
        {
        }

        ram:: ~ram() throw()
        {
            std::cerr << "allocated ram=" << ram_ << std::endl;
        }
        

        const char * ram:: variety() const throw() { return "RAM"; }

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
            if(size>ram_) system_error::critical_bsd(EACCES,"ram::release(more than allocated)");
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

}

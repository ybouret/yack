
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/ram.hpp"
#include <new>
#include <iostream>

namespace yack
{
    namespace memory
    {
        
        static void *ram__[ YACK_WORDS_FOR(ram) ];
        
        global:: global() throw() : allocator(), singleton<global>(),
        initial( memory::ram::allocated() )
        {
            new (out_of_reach::zset(ram__,sizeof(ram__))) ram();
        }
        
        global:: ~global() throw()
        {
            const uint64_t left = memory::ram::allocated();
            if(left>initial)
            {
                std::cerr << "[" << call_sign <<"] allocated=" << left-initial << std::endl;
            }
            out_of_reach::zset( destructed( coerce_cast<ram>(ram__) ), sizeof(ram__) );
        }
        
        void *global:: acquire(size_t &count, const size_t block_size)
        {
            YACK_LOCK(access);
            return coerce_cast<ram>(ram__)->acquire(count,block_size);
        }
        
        void global:: release(void *&addr, size_t &size) throw()
        {
            YACK_LOCK(access);
            coerce_cast<ram>(ram__)->release(addr,size);
        }

        const char   global:: call_sign[] = "memory::global";
        const char * global:: variety() const throw() { return call_sign; }

        
    }
    
}

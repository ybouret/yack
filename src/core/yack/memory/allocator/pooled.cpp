
#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/parcels.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{
    namespace memory
    {
        
        const char pooled:: call_sign[] = "memory::pooled";
        
        static void *parcels__[ YACK_WORDS_FOR(parcels) ];
        
        pooled:: ~pooled() throw()
        {
            out_of_reach::zset( destructed( coerce_cast<parcels>(parcels__) ), sizeof(parcels__) );
        }
        
        pooled:: pooled() : allocator(), singleton<pooled>()
        {
            new (out_of_reach::zset(parcels__,sizeof(parcels__))) parcels();
        }
        
        const char * pooled:: variety() const throw() { return call_sign; }
        
        void *pooled:: acquire(size_t &count, const size_t block_size)
        {
            
        }
        
        void pooled:: release(void *&addr, size_t &size) throw()
        {
            
        }
        
    }
    
}



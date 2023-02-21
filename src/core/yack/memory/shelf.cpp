
#include "yack/memory/shelf.hpp"
#include "yack/memory/allocator.hpp"
#include "yack/memory/embed.hpp"

namespace yack
{
    namespace memory
    {
        shelf:: ~shelf() noexcept
        {
            empty();
        }
        
        shelf:: shelf() noexcept :
        bytes(0),
        entry(0),
        owner(0)
        {
        }
        
        shelf:: shelf(const void  *addr,
                      const size_t size,
                      allocator   &user) noexcept :
        bytes(0),
        entry(0),
        owner(0)
        {
            assert( yack_good(addr,size) );
            entry         = (void *)addr;
            coerce(bytes) = size;
            owner         = &user;
        }
        
        void shelf:: trade(shelf &other) noexcept
        {
            coerce_cswap(bytes,other.bytes);
            coerce_cswap(entry,other.entry);
            coerce_cswap(owner,other.owner);
        }
        

        
        void shelf:: empty() noexcept
        {
            if(bytes)
            {
                assert(entry);
                assert(owner);
                owner->release(entry,coerce(bytes));
            }
        }

        void shelf:: store(const void  *addr,
                           const size_t size,
                           allocator   &user) noexcept
        {
            assert( yack_good(addr,size) );
            shelf target(addr,size,user);
            trade(target);
        }

        void shelf:: build(embed *emb, const size_t num, allocator &user)
        {
            assert(0 != emb);
            assert(num>0);
            
            shelf  target(0,0,user);
            target.entry = embed::zalloc(emb,num,user,coerce(target.bytes));
            trade(target);
        }
        
    }
}

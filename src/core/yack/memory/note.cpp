
#include "yack/memory/note.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"
#include "yack/memory/small/arena.hpp"
#include "yack/memory/allocator/global.hpp"

namespace yack
{

    namespace memory
    {

        note:: ~note() throw()
        {
            size_t i = parts;
            while(i>0)
            {
                destruct( &self[--i] );
            }
            out_of_reach::zset(impl,sizeof(impl));
            self = NULL;
        }

        note:: note() : self(NULL), impl()
        {
            static allocator  &hmem    = global::instance();
            static const bool  compact = true;

            self = coerce_cast<arena>( out_of_reach::zset(impl,sizeof(impl)) );
            size_t na = 0;
            size_t bs = 1;
            try
            {
                while(na<parts)
                {
                    new (self+na) arena(bs,hmem,compact);
                    ++na;
                    bs <<= 1;
                }

            }
            catch(...)
            {
                while(na>0)
                {
                    out_of_reach::zset( destructed(&self[--na]), sizeof(arena) );
                }
                throw;
            }
        }

        void * note:: query(const size_t page_exp2)
        {
            assert(page_exp2<=max_page_exp2);
            return self[page_exp2].acquire();
        }

        void note:: store(void *addr, const size_t page_exp2) throw()
        {
            assert(addr!=NULL);
            assert(page_exp2<=max_page_exp2);
            return self[page_exp2].release(addr);
        }
        
        
    }

}

#include <iostream>
namespace yack
{

    namespace memory
    {
        void note::display() const
        {
            std::cerr << "  <note range=\"2^0->2^" << max_page_exp2 << "\">" << std::endl;
            for(size_t i=0;i<parts;++i)
            {
                self[i].display();
            }
            std::cerr << "  <note/>" << std::endl;
        }

    }
}


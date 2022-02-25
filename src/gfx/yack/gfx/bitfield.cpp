#include "yack/gfx/bitfield.hpp"
#include "yack/memory/allocator/dyadic.hpp"

#include <cstring>

namespace yack
{
    namespace graphic
    {

        bitfield:: ~bitfield() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            if(memio)
            {
                destruct(memio);
                memio = 0;
                memset(wksp,0,sizeof(wksp));
            }
            mgr.release(entry, coerce(bytes) );
        }

        static void *bitfield_acquire(size_t &bytes)
        {
            static memory::allocator &mgr = memory::dyadic::instance();
            return mgr.acquire(bytes,1);
        }

        bitfield:: bitfield(const size_t n) :
        bytes( n ),
        entry( bitfield_acquire( coerce(bytes) ) ),
        memio(NULL),
        wksp()
        {
            memset(wksp,0,sizeof(wksp));
        }


    }
}

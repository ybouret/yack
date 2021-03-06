
#include "yack/string/string_.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/allocator/legacy.hpp"
#include "yack/memory/joint/parcel.hpp"

#include <cstring>
namespace yack
{

    namespace kernel
    {
        string_:: ~string_() throw()
        {
            static memory::allocator &mgr = memory::pooled::location();
            assert(block);
            assert(width+1==space);
            chars = memory::parcel::size_of(block);
            assert(chars>0);
            assert(0==(chars%space));
            mgr.release(block,chars);
            coerce(width)=0;
            coerce(space)=0;
        }

        namespace
        {
            static inline
            void *string_acquire(size_t &width, size_t &space, size_t bpc)
            {
                assert(bpc>0);
                static memory::allocator &mgr = memory::pooled::instance();

                void *ptr = mgr.acquire(space=width+1,bpc);
                space/=bpc; assert(space>width);
                width=space-1;
                return ptr;
            }
        }


        string_:: string_(const size_t num, const size_t bpc) :
        object(),
        counted(),
        memory::ro_buffer(),
        chars(0),
        width(num),
        space(0),
        block(string_acquire(coerce(width),coerce(space),bpc))
        {
        }

        string_:: string_(const string_ &str,
                          const size_t   bpc) :
        object(),
        counted(),
        memory::ro_buffer(),
        chars(str.chars),
        width(str.chars),
        space(0),
        block(string_acquire(coerce(width),coerce(space),bpc))
        {
            memcpy(block,str.block,chars*bpc);
        }


        const void * string_:: ro_addr() const throw()
        {
            return block;
        }

        void string_:: swap_with(string_ &other) throw()
        {
            cswap(chars,other.chars);
            coerce_cswap(width,other.width);
            coerce_cswap(space,other.space);
            cswap(block,other.block);
        }

        

    }
}




#include "yack/math/algebra/lu-kernel.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/memory/embed.hpp"

namespace yack
{
    namespace math
    {

        size_t LU_Kernel:: granted() const noexcept
        {
            return wlen;
        }

        LU_Kernel:: ~LU_Kernel() noexcept
        {
            if(nmax>0)
            {
                static memory::allocator &mgr = memory::dyadic::location();
                indx_ = 0;
                scal_ = 0;
                xtra_ = 0;
                coerce(nmax) = 0;
                mgr.release(wksp,wlen);
            }

        }

        LU_Kernel:: LU_Kernel(const size_t maximum_dim,
                              const size_t scalar_size,
                              const size_t inside_size) :
        nmax( maximum_dim ),
        dneg(false),
        indx_(0),
        scal_(0),
        xtra_(0),
        wksp(0),
        wlen(0)
        {
            if(nmax>0)
            {
                assert(scalar_size>0);
                assert(inside_size>=scalar_size);

                static memory::allocator &mgr   = memory::dyadic::instance();
                memory::embed             emb[] =
                {
                    memory::embed(indx_,nmax),
                    memory::embed(scal_,scalar_size*nmax,as_capacity),
                    memory::embed(xtra_,inside_size*nmax,as_capacity)
                };
                wksp = YACK_MEMORY_EMBED(emb,mgr,wlen);
            }
        }


    }
}

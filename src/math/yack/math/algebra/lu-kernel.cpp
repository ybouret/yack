#include "yack/math/algebra/lu-kernel.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/memory/embed.hpp"

namespace yack
{
    namespace math
    {

        size_t lu_kernel:: granted() const noexcept
        {
            return wlen;
        }

        lu_kernel:: ~lu_kernel() noexcept
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

        lu_kernel:: lu_kernel(const size_t maxi_dims,
                              const size_t scal_size,
                              const size_t type_size) :
        nmax( maxi_dims ),
        dneg(false),
        indx_(0),
        scal_(0),
        xtra_(0),
        wksp(0),
        wlen(0)
        {
            if(nmax>0)
            {
                assert(scal_size>0);
                assert(type_size>0);

                static memory::allocator &mgr = memory::dyadic::instance();


                memory::embed emb[] =
                {
                    memory::embed(indx_,nmax),
                    memory::embed(scal_,scal_size*nmax,as_capacity),
                    memory::embed(xtra_,type_size*nmax,as_capacity)
                };

                wksp = YACK_MEMORY_EMBED(emb,mgr,wlen);
            }
        }


    }
}

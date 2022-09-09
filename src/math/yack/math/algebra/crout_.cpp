#include "yack/math/algebra/crout_.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/memory/embed.hpp"

namespace yack
{
    namespace math
    {

        size_t crout_:: granted() const throw() { return wlen; }

        crout_:: ~crout_() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            coerce(dims) = 0;
            indx_        = 0;
            scal_        = 0;
            xtra_        = 0;
            mgr.release(wksp,wlen);
        }

        crout_:: crout_(const size_t dimension,
                        const size_t type_size,
                        const size_t scal_size) :
        dims( dimension ),
        indx_(0),
        scal_(0),
        xtra_(0)
        {
            static memory::allocator &mgr = memory::dyadic::instance();

            memory::embed emb[] =
            {
                memory::embed(indx_,dims),
                memory::embed(scal_,scal_size*dims,as_capacity),
                memory::embed(xtra_,type_size*dims,as_capacity)
            };

            wksp = YACK_MEMORY_EMBED(emb,mgr,wlen);

        }


    }

}

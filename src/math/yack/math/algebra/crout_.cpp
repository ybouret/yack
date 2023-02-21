#include "yack/math/algebra/crout_.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/memory/embed.hpp"
#include <iostream>

namespace yack
{
    namespace math
    {

        size_t crout_:: granted() const noexcept { return wlen; }

        crout_:: ~crout_() noexcept
        {
            static memory::allocator &mgr = memory::dyadic::location();
            coerce(nmax) = 0;
            indx_        = 0;
            scal_        = 0;
            xtra_        = 0;
            mgr.release(wksp,wlen);
        }

        crout_:: crout_(const size_t dimension,
                        const size_t type_size,
                        const size_t scal_size) :
        nmax( dimension ),
        dneg(false),
        indx_(0),
        scal_(0),
        xtra_(0),
        wksp(0),
        wlen(0)
        {
            static memory::allocator &mgr = memory::dyadic::instance();

            memory::embed emb[] =
            {
                memory::embed(indx_,nmax),
                memory::embed(scal_,scal_size*nmax,as_capacity),
                memory::embed(xtra_,type_size*nmax,as_capacity)
            };

            wksp = YACK_MEMORY_EMBED(emb,mgr,wlen);

#if 0
            std::cerr << "indx_size=" << sizeof(size_t) << std::endl;
            std::cerr << "scal_size=" << scal_size << std::endl;
            std::cerr << "type_size=" << type_size << std::endl;
            
            for(size_t i=0;i<sizeof(emb)/sizeof(emb[0]);++i)
            {
                std::cerr << emb[i] << std::endl;
            }
            
            std::cerr << "indx_@" << (void*)indx_ << std::endl;
            std::cerr << "scal_@" << (void*)scal_ << std::endl;
            std::cerr << "xtra_@" << (void*)xtra_ << std::endl;

            exit(0);
#endif
        }


    }

}

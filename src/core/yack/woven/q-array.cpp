

#include "yack/woven/q-array.hpp"
#include "yack/memory/embed.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/apex/flak.hpp"
#include "yack/sequence/thin-array.hpp"

namespace yack
{
    namespace woven
    {
        q_array:: q_array(const size_t dims) :
        metrics(dims),
        next(0),
        prev(0),
        coord(0),
        znrm2(0),
        entry(0),
        bytes(0)
        {
            init();
        }



        q_array:: ~q_array() noexcept {
            quit(dimensions);
        }

        size_t q_array:: granted() const noexcept
        {
            return bytes;
        }

        void q_array:: quit(size_t n) noexcept
        {
            static memory::allocator &mgr = memory::dyadic::location();
            destruct(znrm2);
            while(n>0) {
                destruct(&coord[n--]);
            }
            znrm2 = 0;
            coord = 0;
            mgr.release(entry,bytes);

        }

        void q_array:: init()
        {
            // get allocator
            static memory::allocator &mgr = memory::dyadic::instance();

            // prepare embed layut
            memory::embed emb[] =
            {
                memory::embed(coord,dimensions),
                memory::embed(znrm2,1)
            };

            // get linear space
            entry = YACK_MEMORY_EMBED(emb,mgr,bytes);

            // populate linear space
            try {
                new (znrm2) apz();
                apq   *q     = coord--;
                size_t built = 0;
                try
                {

                    while(built<dimensions)
                    {
                        new (q+built) apq();
                        ++built;
                    }
                }
                catch(...)
                {
                    quit(built);
                    throw;
                }
            }
            catch(...) {
                mgr.release(entry,bytes);
                throw;
            }
        }

        size_t q_array:: size() const noexcept
        {
            return dimensions;
        }

        const apq & q_array:: operator[](const size_t i) const noexcept
        {
            assert(i>=1);
            assert(i<=dimensions);
            return coord[i];
        }

        void q_array:: finalize()
        {
            thin_array<apq> self(&coord[1],dimensions);
            apex::flak::univocal(self);
            apz &norm2 = *znrm2; assert(0==norm2);
            for(size_t i=dimensions;i>0;--i)
                norm2 += coord[i].num.mod2();
        }

        bool q_array::check_not_null(const readable<apq> &Q) noexcept
        {
            for(size_t i=Q.size();i>0;--i)
            {
                if(__zero__!=Q[i].num.s) return true;
            }
            return false;
        }


    }

}



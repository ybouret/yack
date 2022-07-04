#include "yack/chem/mix/groups.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/counting/mloop.hpp"

namespace yack
{

    namespace chemical
    {


        void groups:: weave(const readable<groups> &part)
        {
            assert(0==size);
            assert(0<part.size());
            const size_t            dim = part.size();
            const cxx_array<size_t> ini(dim,1);
            cxx_array<size_t>       end(dim,0);
            for(size_t i=dim;i>0;--i) end[i] = part[i].size;
            mloop<size_t>           loop( ini(), end(), dim);
            YACK_CHEM_PRINTLN("weaving " << end << " -> #" << loop.total);
            do
            {
                group *g = push_back( new group() );
                for(size_t i=1;i<=dim;++i)
                {
                    const groups &G = part[i];
                    const size_t  j = loop[i];
                    const group  &p = *G.get(j);
                    g->merge_back_copy(p);
                }
            }
            while(loop.next());

            sort();
            std::cerr << *this << std::endl;
        }

    }

}


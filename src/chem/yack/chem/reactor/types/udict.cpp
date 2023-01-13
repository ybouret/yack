
#include "yack/chem/reactor/types/udict.hpp"
#include "yack/system/imported.hpp"


namespace yack {

    namespace chemical {

        static const char fn[] = "chemical::dictionary";


        umap:: umap() throw() : large_object(), umap_() {}

        umap:: ~umap() throw()
        {
        }

    }

}

#include <cstring>

namespace yack {

    namespace chemical {


        udict:: udict(const char *which) : fwd( new umap() ), rev( new umap() )
        {
            assert(which);
            memset(kind,0,sizeof(kind));
            memcpy(kind,which, min_of<size_t>(strlen(which),sizeof(kind)-1));
        }

        udict:: ~udict() throw()
        {
            memset(kind,0,sizeof(kind));
        }

        void udict:: throw_multiple_forward(const size_t i) const {
            throw imported::exception(fn,"multiple forward %u for %s",unsigned(i),kind);
        }

        void udict:: throw_multiple_reverse(const size_t I) const {
            throw imported::exception(fn,"multiple reverse %u for %s",unsigned(I),kind);
        }

        size_t udict:: forward(const size_t i) const
        {
            const size_t *I = fwd->search(i); if(!I)  throw imported::exception(fn,"no %s forward index %u",kind,unsigned(i));
            return *I;
        }

        size_t udict:: reverse(const size_t I) const
        {
            const size_t *i = rev->search(I); if(!i)  throw imported::exception(fn,"no %s reverse index %u",kind,unsigned(I));
            return *i;
        }

    }

}

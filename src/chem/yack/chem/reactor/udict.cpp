
#include "yack/chem/reactor/udict.hpp"



namespace yack {

    namespace chemical {

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
            std::cerr << "sizeof(kind)=" << sizeof(kind) << std::endl;
        }

        udict:: ~udict() throw()
        {
        }

    }

}

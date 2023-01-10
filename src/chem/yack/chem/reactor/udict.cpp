
#include "yack/chem/reactor/udict.hpp"



namespace yack {

    namespace chemical {

        umap:: umap() throw() : large_object(), umap_() {}

        umap:: ~umap() throw()
        {
        }

    }

}


namespace yack {

    namespace chemical {

        udict:: udict() : fwd( new umap() ), rev( new umap() ) {}

        udict:: ~udict() throw()
        {
        }

    }

}

#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {
        void cluster:: make_manifold(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"cluster::make_manifold");
            
            const size_t n = (*group)->size; assert(n>0);
            const size_t m = (*alive)->size; assert(m>=n);
            
            
        }
        
    }
    
}


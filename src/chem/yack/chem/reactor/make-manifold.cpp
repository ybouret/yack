#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {
        void cluster:: make_manifold(const xmlog &xml, const matrix<int> &Nu)
        {
            YACK_XMLSUB(xml,"cluster::make_manifold");
            
            const size_t n = (*group)->size; assert(n>0);
            const size_t m = (*alive)->size; assert(m>=n);
            matrix<int>  nu(n,m);
            for(const gnode *en=(*group)->head;en;en=en->next)
            {
                const equilibrium   &eq = en->host;
                const size_t         eI = *eq;
                const size_t         ei = **en;
                const readable<int> &nI = Nu[eI];
                writable<int>       &ni = nu[ei];
                for(const anode *sn=(*alive)->head;sn;sn=sn->next)
                {
                    const species &sp = sn->host;
                    const size_t   sJ = *sp;
                    const size_t   sj = **sn;
                    ni[sj] = nI[sJ];
                }
            }
            YACK_XMLOG(xml, "nu=" << nu);
        }
        
    }
    
}


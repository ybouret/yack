
#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/data/small/repo.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;
    
    namespace chemical
    {
        void nexus:: conserved_set(const xmlog &xml)
        {
            static const char * const fn = "conserved_set";
            YACK_XMLSUB(xml,fn);
            
            if(regular.size<=0)
            {
                YACK_XMLOG(xml, "-- no regular equilibrium");
                return;
            }
            
            matrix<apq> P(regular.size,M);
            for(const eq_node *en=regular.head;en;en=en->next)
            {
                const equilibrium   &eq = **en;
                const size_t         ei = *eq;
                const readable<int> &nu = Nu[ei];
                writable<apq>       &Pi = P[ei];
                for(const cnode *cn=eq.head();cn;cn=cn->next)
                {
                    const species &s = ****cn;
                    const size_t   j = *s;
                    if(conserved==crit[j]) Pi[j] = nu[j];
                }
            }
            std::cerr << "P=" << P << std::endl;
            matrix<apq> Q(M,M);
            if( !ortho_family::build(Q,P) )
            {
                YACK_XMLOG(xml, "-- singular conserved set");
                return;
            }
            std::cerr << "Q=" << Q << std::endl;

            
        }
    
                                
        
    }
    
}


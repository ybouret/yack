
#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/sequence/cxx-series.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;
    
    namespace chemical
    {

        static inline apz q2z(const apq &q)
        {
            assert(1==q.den);
            return q.num;
        }

        void nexus:: conserved_set(const xmlog &xml)
        {
            static const char * const fn = "conserved_set";
            YACK_XMLSUB(xml,fn);

            const size_t n = regular.size;
            if(n<=0)
            {
                YACK_XMLOG(xml, "-- no regular equilibrium");
                return;
            }

            YACK_XMLOG(xml, "-- selecting conserved species");
            const matrix<apq> P(n,M);
            {
                size_t             ii=1;
                for(const eq_node *en=regular.head;en;en=en->next,++ii)
                {
                    const equilibrium   &eq = **en;
                    const size_t         ei = *eq;
                    const readable<int> &nu = Nu[ei];
                    writable<apq>       &Pi = coerce(P[ii]);
                    for(const cnode *cn=eq.head();cn;cn=cn->next)
                    {
                        const species &s = ****cn;
                        const size_t   j = *s;
                        if(conserved==crit[j]) Pi[j] = nu[j];
                    }
                }
            }
            if(verbose) std::cerr << "P=" << P << std::endl;
            const matrix<apq> Q(M,M);
            if( !ortho_family::build(coerce(Q),P) )
            {
                YACK_XMLOG(xml, "-- singular conserved set");
                return;
            }
            std::cerr << "Q=" << Q << std::endl;
            const size_t  rank = apk::rank_of(Q);
            std::cerr << "rank(Q) = " << rank << std::endl;


            cxx_series<size_t> irow(M);
            cxx_series<size_t> isub(M-1);

            {
                matrix<apz> ZQ(q2z,Q);

                //! select rows
                for(size_t i=1;i<=M;++i)
                {
                    bool ok = true;
                    for(size_t j=irow.size();j>0;--j)
                    {
                        const size_t jr = irow[j];
                        if( apk::are_prop(ZQ[jr],ZQ[i],NULL))
                        {
                            ok = false;
                            break;
                        }
                    }
                    if(ok)
                        irow << i;
                }
            }

            std::cerr << "irow=" << irow << std::endl;

            const size_t m = irow.size(); assert(m>=rank);
            matrix<apq>  U(rank,M);

            for(size_t i=1;i<=m;++i)
            {
                const size_t ir = irow[i];
                isub.free();
                for(size_t ii=1;ii<i;++ii)    isub << irow[ii];
                for(size_t ii=i+1;ii<=m;++ii) isub << irow[ii];
                std::cerr << "starting @" << ir << ", sub in " << isub << std::endl;
            }




        }
    
                                
        
    }
    
}


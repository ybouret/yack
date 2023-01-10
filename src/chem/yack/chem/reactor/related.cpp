
#include "yack/chem/reactor.hpp"
#include "yack/apex/alga.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {
        
        static inline
        bool have_common_species(const readable<int> &lhs,
                                 const readable<int> &rhs) throw()
        {
            for(size_t i=lhs.size();i>0;--i)
            {
                if(lhs[i] && rhs[i]) return true;
            }
            return false;
        }
        
        void reactor:: build_related(const xmlog &xml)
        {
            YACK_XMLSUB(xml, "cluster::related" );
            

            {
                //--------------------------------------------------------------
                // build global matrix of related
                //--------------------------------------------------------------
                matrix<bool> related(N,N);
                for(size_t i=1;i<=N;++i)
                {
                    related[i][i] = true;
                    const readable<int> &ni = Nu[i];
                    for(size_t j=i+1;j<=N;++j)
                    {
                        const readable<int> &nj = Nu[j];
                        related[i][j] = related[j][i] = have_common_species(ni,nj);
                    }
                }
                if(verbose)
                {
                    eqs(std::cerr, "",related);
                }

                //--------------------------------------------------------------
                // build clusters
                //--------------------------------------------------------------
                clusters        & cc = coerce( *linked );
                for(const enode * en = eqs.head();en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    bool               ok = false;
                    for(cluster *cl=cc.head;cl;cl=cl->next)
                    {
                        if( cl->linked_with(eq,related) ) {
                            cl->grow(eq);
                            ok = true;
                            break;
                        }
                    }

                    if(!ok) cc.push_back( new cluster(eq) );
                }
            }
            std::cerr << linked << std::endl;

            for(cluster *cl = linked->head;cl;cl=cl->next)
            {
                cl->compile(xml);
            }

        }

    }
    
}


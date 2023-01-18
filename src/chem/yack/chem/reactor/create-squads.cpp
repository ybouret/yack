#include "yack/chem/reactor.hpp"

namespace yack
{
    namespace chemical
    {
        namespace
        {
            static inline bool have_common(const readable<int> &lhs,
                                           const readable<int> &rhs) throw()
            {
                assert( lhs.size() == rhs.size() );
                for(size_t i=lhs.size();i>0;--i) {
                    if( (lhs[i] != 0) && (rhs[i] != 0) ) return true;
                }
                return false;
            }

            static inline
            void fill_detached(matrix<bool> &detached, const matrix<int> &Nu ) throw()
            {
                detached.ld(false);
                const size_t r=Nu.rows;
                for(size_t i=1;i<r;++i)
                {
                    const readable<int> &lhs = Nu[i];
                    for(size_t j=i+1;j<=r;++j)
                    {
                        const readable<int> &rhs = Nu[j];
                        detached[i][j] = detached[j][i] = !have_common(lhs,rhs);
                    }
                }
            }
        }

        void reactor:: create_squads(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"cluster::squads");
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            const size_t dim = all.size();
            matrix<bool> detached(dim,dim);
            matrix<int>  topology(dim,Nu.cols);
            eq_repo_     assembly;

            //------------------------------------------------------------------
            //
            YACK_XMLOG(xml, "--> create global topology");
            //
            //------------------------------------------------------------------
            for(const enode *en=all.head();en;en=en->next)
            {
                const equilibrium &eq = ***en;
                eq.fill(topology[*eq]);
            }
            fill_detached(detached,topology); //if(xml.verbose) all(*xml << "detached=","",detached);


            //------------------------------------------------------------------
            //
            YACK_XMLOG(xml, "--> create one army per cluster");
            //
            //------------------------------------------------------------------
            for(cluster *cc=linked->head;cc;cc=cc->next)
            {
                //--------------------------------------------------------------
                // make army
                //--------------------------------------------------------------
                cc->assemble_all_group(assembly);                // create assembly
                coerce( *(cc->army) ).shape(assembly,detached);  // assembly+detached => army

                cc->army->print(xml,"cluster::army");

                //--------------------------------------------------------------
                // soften topology: removing unbounded species
                //--------------------------------------------------------------
                for(const sp_gnode *sn=cc->replica->breed->unbounded->head;sn;sn=sn->next)
                {
                    const species &s  = (***sn).host;
                    const size_t   j  = *s;
                    for(size_t i=dim;i>0;--i) {
                        topology[i][j] = 0;
                    }
                    YACK_XMLOG(xml, " (-) '" << s.name << "'");
                }
            }

            //------------------------------------------------------------------
            //
            YACK_XMLOG(xml, "--> create one wing per cluster");
            // using only balancing equilibria
            //
            //------------------------------------------------------------------
            fill_detached(detached,topology);
            for(cluster *cc=linked->head;cc;cc=cc->next)
            {
                coerce( *(cc->wing) ).shape(*(cc->replica->genus->balancing),detached);  // assembly+detached => wing
                cc->wing->print(xml,"cluster::wing");
            }
        }

        
    }

}

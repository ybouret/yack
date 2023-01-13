
#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {

        void cluster:: make_detached(const xmlog &xml, const equilibria &all)
        {
            static const char * const fn = "cluster::make_detached";
            YACK_XMLSUB(xml,fn);

            //------------------------------------------------------------------
            //
            // prepare resources
            //
            //------------------------------------------------------------------
            const glist &grp = *group;
            const size_t n   = grp->size; assert(n>0);

            matrix<bool> detached(n,n); detached.ld(false);
            eq_repo_     assembly;

            //------------------------------------------------------------------
            //
            // form strict detached matrix
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- building detached matrix");
            for(const gnode *gn=grp->head;gn;gn=gn->next)
            {
                const equilibrium &g = gn->host;
                const size_t       i = **gn;
                assembly << g;
                for(const gnode *hn=gn->next;hn;hn=hn->next)
                {
                    const equilibrium &h = hn->host;
                    const size_t       j = **hn;
                    if( g.detached_of(h) ) {
                        detached[i][j] = detached[j][i] = true;
                    }
                }
            }

            if(xml.verbose) {
                for(const gnode *gn=grp->head;gn;gn=gn->next)
                {
                    const equilibrium &g  = gn->host;
                    const size_t       i = **gn;
                    all.pad(*xml << '<' << g.name << '>',g) << " : " << detached[i] << std::endl;
                }
            }

            //------------------------------------------------------------------
            //
            // build army
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- building army of squads");
            coerce(*army).shape(assembly,detached);


            

        }
    }

}

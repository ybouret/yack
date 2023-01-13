
#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {

        void cluster:: assemble_all_group(eq_repo_ &assembly) const
        {
            assembly.clear();
            for(const gnode *gn=(*group)->head;gn;gn=gn->next)
                assembly << gn->host;
        }

        void cluster:: assemble_delimited(eq_repo_ &assembly) const
        {
            assembly.clear();
            for(const eq_gnode *en=genus->delimited->head;en;en=en->next)
                assembly << (***en).host;
        }


        void cluster:: make_detached(const xmlog        &xml,
                                     squads             &platoon,
                                     const matrix<bool> &detached,
                                     const char * const  info)
        {
            YACK_XMLSUB(xml,info);

            //------------------------------------------------------------------
            //
            // prepare resources
            //
            //------------------------------------------------------------------
            const glist &grp = *group;
            eq_repo_     assembly;

            //------------------------------------------------------------------
            //
            // form assembly
            //
            //------------------------------------------------------------------
            for(const gnode *gn=grp->head;gn;gn=gn->next)
                assembly << gn->host;
            

            //------------------------------------------------------------------
            //
            // build army
            //
            //------------------------------------------------------------------
            platoon.shape(assembly,detached);
            if(xml.verbose)
            {
                for(const squad *sq = platoon.head;sq;sq=sq->next)
                {
                    *xml << *sq << std::endl;
                }
            }

        }

    }

}

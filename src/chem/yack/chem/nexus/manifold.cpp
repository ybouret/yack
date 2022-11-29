#include "yack/chem/nexus.hpp"

namespace yack
{
    namespace chemical
    {

        void nexus:: make_manifold(const xmlog &xml)
        {
            static const char * const fn = "make_manifold";
            YACK_XMLSUB(xml,fn);

            //------------------------------------------------------------------
            //
            // outer loop: use a cluster of related equilibria
            //
            //------------------------------------------------------------------
            for(const cluster *sharing=related.head;sharing;sharing=sharing->next)
            {

            }




        }

    }

}


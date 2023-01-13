
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
            {
                assembly << (***en).host;
            }
        }

    }

}

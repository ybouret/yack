#include "yack/chem/reactor/cluster.hpp"

namespace yack
{
    namespace chemical
    {
        
        static inline int compare_sp(const species &lhs, const species &rhs) throw()
        {
            return comparison::increasing(*lhs,*rhs);
        }


        static inline void populate(sp_repo &slist, const addrbook &tribe)
        {
            for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                slist << *static_cast<const species *>(*it);
            slist.sort_with(compare_sp);
        }

        void cluster:: collect_alive()
        {

            sp_repo slist;

            //------------------------------------------------------------------
            // collect all species
            //------------------------------------------------------------------
            {
                addrbook tribe;
                for(const gnode *gn = (*group)->head;gn;gn=gn->next)
                    gn->host.update(tribe);
                populate(slist,tribe);
            }

            //------------------------------------------------------------------
            // transfert to active list
            //------------------------------------------------------------------
            alist &my = coerce( *alive );
            for(const sp_node *sn=slist.head;sn;sn=sn->next)
                my << ***sn;
        }

    }

}


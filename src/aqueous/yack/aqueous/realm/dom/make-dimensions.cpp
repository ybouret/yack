
#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace aqueous
    {
        static inline int compare_sp_nodes(const sp_node *lhs,
                                           const sp_node *rhs) noexcept
        {
            const species &L = ***lhs;
            const species &R = ***rhs;
            return comparison::increasing(L.indx[0],R.indx[0]);
        }
        
        void domain:: make_dimensions(const xmlog &xml)
        {
             addrbook db;

            //------------------------------------------------------------------
            // register all species and setup equilibria local indices
            //------------------------------------------------------------------
            {
                size_t   ei = 0;
                for(const eq_node *en=head;en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    if(eq->size<=0) throw imported::exception(clid,"<%s> is empty!!", eq.name());
                    eq.report_to(db);
                    coerce(eq.indx[1]) = ++ei;
                }
                coerce(N)=ei;
            }

            //------------------------------------------------------------------
            // extract all species
            //------------------------------------------------------------------
            sp_list &mine = coerce(live);
            mine.clear();
            for(addrbook::const_iterator it=db.begin();it!=db.end();++it)
            {
                mine << *static_cast<species *>( *it );
            }

            //------------------------------------------------------------------
            // sort live
            //------------------------------------------------------------------
            merge_list_of<sp_node>::sort(mine,compare_sp_nodes);

            //------------------------------------------------------------------
            // setup species local indices
            //------------------------------------------------------------------
            {
                size_t sj = 0;
                for(sp_node *sn=mine.head;sn;sn=sn->next)
                {
                    const species &sp = ***sn;
                    coerce(sp.indx[1]) = ++sj;
                }
                coerce(M)=sj;
            }

            YACK_XMLOG(xml,"live = " <<live);
        }
    }

}



#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace aqueous
    {
       

        void domain:: make_dimensions(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"make_dimensions");
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
                    coerce(eqfmt).update(eq);
                };
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
            sort_top_level(mine);

            //------------------------------------------------------------------
            // setup species local indices
            //------------------------------------------------------------------
            {
                size_t sj = 0;
                for(sp_node *sn=mine.head;sn;sn=sn->next)
                {
                    const species &sp = ***sn;
                    coerce(sp.indx[sub_level]) = ++sj;
                    coerce(spfmt).update(sp);
                }
                coerce(M)=sj;
            }

            YACK_XMLOG(xml,"live = " <<live);
            if(M<N) throw imported::exception(clid,"too many equilibria!!");
        }
    }

}


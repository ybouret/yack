#include "yack/aqueous/realm/collector.hpp"
#include "yack/system/imported.hpp"
#include <iomanip>

namespace yack
{

    namespace aqueous
    {
        double collector:: combined(const cluster &clan)
        {
            xadd.free();
            for(const eq_node *en = clan.head; en; en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = eq.indx[cat_level];
                const double       g  = gain[ei];assert(g>=0);
                xadd.push(g);
            }
            return xadd.sum();
        }

        const cluster & collector:: find_opt(const partition        &part,
                                             const eq_repo          &zeqs,
                                             const xmlog            &xml )
        {
            YACK_XMLSUB(xml, "gain_for_each_subset");

            assert(part.size>0);
            assert(zeqs.size>0);
            const cluster *win = part.head;
            double         opt = 0;

        FIRST_WINNER:
            if( !win->is_subset_of(zeqs) )
            {
                win = win->next;
                if(!win) throw imported::exception("aqueous::collector","no initial subset: corrupted code");
                goto FIRST_WINNER;
            }
            assert(NULL!=win);
            opt = combined(*win);

            YACK_XMLOG(xml, "(+) " << std::setw(15) << opt << " @" << *win);
            for(const cluster *cls=win->next;cls;cls=cls->next)
            {
                if(!cls->is_subset_of(zeqs)) continue;
                const double tmp = combined(*cls);
                if(tmp>opt)
                {
                    opt = tmp;
                    win = cls;
                    YACK_XMLOG(xml, "(+) " << std::setw(15) << opt << " @" << *win);
                }
                else
                {
                    YACK_XMLOG(xml, "(-) " << std::setw(15) << tmp << " @" << *cls);
                }
            }

            YACK_XMLOG(xml, "--> " << std::setw(15) << opt << " @" << *win);
            return *win;
        }
    }

}



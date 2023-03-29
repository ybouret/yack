#include "yack/aqueous/realm/domain.hpp"

namespace yack
{
    namespace aqueous
    {

        void domain:: make_index_maps(const xmlog      &xml)
        {
            YACK_XMLSUB(xml,"make_index_maps");

            {
                index_map &em = coerce(eqmap);
                for(const eq_node *en=head;en!=last;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       gi = eq.indx[top_level];
                    const size_t       li = eq.indx[sub_level];
                    if(xml.verbose) eqfmt.pad( *xml << eq,eq) << " : " << gi << " => " << li << std::endl;
                    em(gi,li);
                }
            }
            YACK_XMLOG(xml, "|_eqmap = " << eqmap);


            {
                index_map &sm =  coerce(spmap);
                for(const sp_node *sn=live.head;sn;sn=sn->next)
                {
                    const species &sp = ***sn;
                    const size_t   gj = sp.indx[top_level];
                    const size_t   lj = sp.indx[sub_level];
                    if(xml.verbose) spfmt.pad( *xml << sp,sp) << " : " << gj << " => " << lj << std::endl;
                    sm(gj,lj);
                }
            }
            YACK_XMLOG(xml, "|_spmap = " << spmap);

        }
    }

}


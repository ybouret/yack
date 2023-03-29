
#include "yack/aqueous/species.hpp"
#include "yack/ios/ostream.hpp"

namespace yack
{
    namespace aqueous
    {
        bool species::verbose = false;

        static inline int compare_sp_nodes(const sp_node *lhs,
                                           const sp_node *rhs) noexcept
        {
            const species &L = ***lhs;
            const species &R = ***rhs;
            return comparison::increasing(L.indx[0],R.indx[0]);
        }


        void  sort_top_level(sp_list &mine)
        {
            merge_list_of<sp_node>::sort(mine,compare_sp_nodes);
        }

        species:: ~species() noexcept
        {
        }


        std::ostream & operator<<(std::ostream &os, const species &self)
        {
            os << '[' << self.name << ']';
            return os;
        }

        void species:: viz(ios::ostream &fp,
                           const bool    ireg,
                           const size_t  igrp) const
        {
            // use pastel19, 9=>no group=igrp=0
            logo(fp) << '[';
            add_label(fp,name) << ",shape=egg";

            if(ireg)
            {
                fp << ",style=\"bold,filled\"";
            }
            else
            {
                fp << ",style=\"bold,filled,dashed\"";
            }
            {
                const int color = (igrp > 0) ? 1+(igrp%8) : 9;
                fp(",fillcolor=\"/pastel19/%d\"",color);
            }
            fp << ']';
            end(fp);
        }
    }

}



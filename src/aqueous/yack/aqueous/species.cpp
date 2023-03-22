
#include "yack/aqueous/species.hpp"
#include "yack/ios/ostream.hpp"

namespace yack
{
    namespace aqueous
    {
        bool species::verbose = false;
        
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




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

        void species:: viz(ios::ostream &fp, const bool is_reg) const
        {
            logo(fp) << '[';
            add_label(fp,name);

            fp << ']';
            end(fp);
        }
    }

}



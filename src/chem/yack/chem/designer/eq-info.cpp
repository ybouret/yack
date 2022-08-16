
#include "yack/chem/designer/eq-info.hpp"
namespace yack
{
    namespace chemical
    {
        namespace nucleus
        {
            const eq_info eq_info::table[] =
            {
                { "water",    "[H+]+[HO-]",             "10^(-14)"   },

                { "acetic",   "[H+]-[EtCOOH]+[EtCOO-]", "10^(-4.78)" },

                { "ammoniac", "[H+]-[NH4+]+[NH3]",      "10^(-9.25)" },

                { "oxalic1",  "[H+]-[OxH2]+[OxH-]",     "10^(-1.20)" },
                { "oxalic2",  "[H+]-[OxH-]+[Ox--]",     "10^(-4.30)" },

                { "sulfuric", "2[H+]-[H2SO4]+[SO4--]",  "10^(0.89)"  },

                { "benzoic",  "[H+]-[PhiCOOH]+[PhiCOO-]", "10^(-4.202)" },

                { "abcd",     "-[a]-[b]+[c]+[d]", "0.12345" }
            };
            
            
            const size_t eq_info::count = sizeof(table)/sizeof(table[0]);
        }
        
    }
    
}


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

                { "acetic",   "[H+]+[EtCOO-]-[EtCOOH]", "10^(-4.78)" },

                { "ammoniac", "[H+]+[NH3]-[NH4+]",      "10^(-9.25)" },

                { "oxalic1",  "[H+]+[OxH-]-[OxH2]",     "10^(-1.20)" },
                { "oxalic2",  "[H+]+[Ox--]-[OxH-]",     "10^(-4.30)" },

                { "sulfuric", "2[H+]+[SO4--]-[H2SO4]",  "10^(0.89)"  },

                { "benzoic",  "[H+]+[PhiCOO-]-[PhiCOOH]", "10^(-4.202)" }
            };
            
            
            const size_t eq_info::count = sizeof(table)/sizeof(table[0]);
        }
        
    }
    
}

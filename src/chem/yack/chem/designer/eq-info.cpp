
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

                { "citric1",  "[H+]-[CitH3]+[CitH2-]",    "10^(-3.13)" },
                { "citric2",  "[H+]-[CitH2-]+[CitH--]",   "10^(-4.76)" },
                { "citric3",  "[H+]-[CitH--]+[Cit---]",   "10^(-6.40)" },


                { "EDTA1",  "[H+]-[YH4]+[YH3-]",       "10^(-2.00)" },
                { "EDTA2",  "[H+]-[YH3-]+[YH2--]",     "10^(-2.70)" },
                { "EDTA3",  "[H+]-[YH2--]+[YH---]",    "10^(-6.20)" },
                { "EDTA4",  "[H+]-[YH---]+[Y----]",    "10^(-10.30)" },


                { "phosphoric1",  "[H+]-[H3PO4]+[H2PO4-]",    "10^(-2.15)" },
                { "phosphoric2",  "[H+]-[H2PO4-]+[HPO4--]",   "10^(-7.20)" },
                { "phosphoric3",  "[H+]-[HPO4--]+[PO4---]",   "10^(-12.42)" },

                { "abcd",     "-[a]-[b]+[c]+[d]", "0.12345" }
            };
            
            
            const size_t eq_info::count = sizeof(table)/sizeof(table[0]);
        }
        
    }
    
}

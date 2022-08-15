
#include "yack/chem/designer/eq-info.hpp"
namespace yack
{
    namespace chemical
    {
        namespace nucleus
        {
            const eq_info eq_info::table[] =
            {
                { "water",  "[H+]+[HO-]",             "10^(-14)"   },
                { "acetic", "[H+]+[EtCOO-]-[EtCOOH]", "10^(-4.78)" }
            };
            
            
            const size_t eq_info::count = sizeof(table)/sizeof(table[0]);
        }
        
    }
    
}

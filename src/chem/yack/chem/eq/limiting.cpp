
#include "yack/chem/eq/limiting.hpp"

namespace yack
{

    namespace chemical
    {
        limiting:: limiting(const actor *pa_, const double xi_) throw() : pa(pa_), xi(xi_) {}


        const char * limiting:: text(const status s) throw()
        {

            switch(s)
            {
                case by_none: return "limited by none";
                case by_reac: return "limited by reac";
                case by_prod: return "limited by prod";
                case by_both: return "limited by both";
            }

            return yack_unknown;
        }
    }

}



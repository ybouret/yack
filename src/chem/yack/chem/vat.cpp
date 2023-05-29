
#include "yack/chem/vat.hpp"

namespace yack
{
    namespace Chemical
    {
        Vat:: ~Vat() noexcept
        {
        }

        Vat:: Vat(const xmlog   &xml,
                  Equilibria    &eqs) :
        Clusters()
        {
            makeClusters(xml,eqs);
        }

    }

}


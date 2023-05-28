
#include "yack/chem/vat.hpp"

namespace yack
{
    namespace Chemical
    {
        Vat:: ~Vat() noexcept
        {
        }

        Vat:: Vat(const Library &lib,
                  Equilibria    &eqs,
                  const xmlog   &xml) :
        Clusters()
        {
            makeClusters(xml,eqs);
        }

    }

}


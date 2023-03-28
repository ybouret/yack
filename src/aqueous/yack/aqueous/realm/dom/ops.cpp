
#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace aqueous
    {

        void domain:: shrink(writable<double>       &Corg,
                             const readable<double> &C0) const
        {
            spmap.send(Corg,C0);
        }
    }

}

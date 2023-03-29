
#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace aqueous
    {

        void domain:: shrink(writable<double>       &Csub,
                             const readable<double> &Ctop) const
        {
            spmap.send(Csub,Ctop);
        }

        void domain:: expand(writable<double>       &Ctop,
                             const readable<double> &Csub) const
        {
            spmap.recv(Ctop,Csub);
        }
    }

}

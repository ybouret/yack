#include "yack/chem/realm/domain.hpp"

namespace yack
{
    namespace chemical
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

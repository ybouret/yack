#include "yack/chem/nexus.hpp"

namespace yack
{
    namespace chemical
    {

        void nexus::preserve(writable<double> &C0)
        {
            for(const cluster *cls=related.head;cls;cls=cls->next)
            {
                cls->ct.update(C0,Qc,Qs,Qr,xadd);
            }
        }
    }
}

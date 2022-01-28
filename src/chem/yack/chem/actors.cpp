
#include "yack/chem/actors.hpp"
#include "yack/arith/ipower.hpp"

namespace yack
{

    namespace chemical
    {
        actors:: ~actors() throw()
        {
        }

        actors:: actors() throw()
        {
        }

        double actors:: mass_action(double                  K,
                                    const readable<double> &C) const throw()
        {

            for(const actor *a=head;a;a=a->next)
            {
                const species &sp = **a;   assert(sp.indx>=1); assert(sp.indx<=C.size());
                const size_t   nu = a->nu; assert(nu>0);
                K *= ipower<double,size_t>(C[sp.indx],nu);
            }
            return K;
        }

    }

}

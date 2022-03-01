
#include "yack/chem/actors.hpp"

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

        double actors:: mass_action(double factor, const readable<double> &C) const throw()
        {
            for(const actor *a=head;a;a=a->next)
            {
                factor *= a->mass_action(C);
            }
            return factor;
        }


    }

}


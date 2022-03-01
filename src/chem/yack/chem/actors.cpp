
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

        int actors:: nu() const throw()
        {
            int res = 0;
            for(const actor *a=head;a;a=a->next)
            {
                res += a->coef;
            }
            return res;
        }

        int actors:: dz() const throw()
        {
            int res = 0;
            for(const actor *a=head;a;a=a->next)
            {
                res += a->coef * (**a).z;
            }
            return res;
        }

        void actors:: display(std::ostream &os) const
        {
            if(size)
            {
                const actor *a = head;
                a->display(os,true);
                for(a=a->next;a;a=a->next)
                {
                    a->display(os,false);
                }
            }
            else
            {
                os << '.';
            }
        }


    }

}


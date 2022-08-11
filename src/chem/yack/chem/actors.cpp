

#include "yack/chem/actors.hpp"
#include "yack/type/utils.hpp"
#include "yack/arith/ipower.hpp"

#include <new>

namespace yack
{
    namespace chemical
    {

        actors:: ~actors() throw() {}


        actors:: actors() throw() :
        molecularity(0),
        algebraic_Z(0),
        crew(),
        wksp()
        {

        }


        void actors:: operator()(const species &sp, const unsigned nu)
        {
            assert(nu>0);
#ifndef NDEBUG
            for(const actor *a=crew.head;a;a=a->next)
            {
                assert(&sp != & **a);
            }
#endif
            crew.push_back(new actor(sp,nu) );
            coerce(molecularity) += nu;
            coerce(algebraic_Z) += int(nu)*sp.z;
        }

        void actors:: remove_last() throw()
        {
            assert(crew.size);
            const actor   *last = crew.tail;
            const unsigned nu   = last->nu;
            const int      z    = (**last).z;
            coerce(molecularity) -= nu;
            coerce(algebraic_Z)  -= int(nu)*z;
            delete crew.pop_back();
        }

        double actors:: mass_action(const readable<double> &C,
                                    rmulops                &ops) const
        {
            assert(ops.size()==0||ops.size()==1);
            for(const actor *a=crew.head;a;a=a->next)
            {
                ops.push(C[***a],a->nu);
            }
            return ops.query();
        }

        const xlimit *actors:: genuine_limit(const readable<double> &C) const throw()
        {
            const actor *a = crew.head;
            if(a)
            {
                const actor *id = a;
                double       xi = C[***a]/a->nu;
                for(a=a->next;a;a=a->next)
                {
                    const double xi_tmp = C[***a]/a->nu;
                    if(xi_tmp<xi)
                    {
                        xi = xi_tmp;
                        id = a;
                    }
                }
                return new ( *wksp ) xlimit(*id,xi);
            }
            else
            {
                return NULL;
            }
        }
        
        std::ostream & operator<<(std::ostream &os, const actors &A)
        {
            const actor *a = A.crew.head;
            if(a)
            {
                a->display_first(os);
                for(a=a->next;a;a=a->next)
                {
                    a->display_extra(os);
                }
            }
            else
            {
                os << '.';
            }
            return os;
        }



    }

}


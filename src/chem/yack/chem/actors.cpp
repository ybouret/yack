
#include "yack/chem/actors.hpp"
#include "yack/arith/ipower.hpp"
#include "yack/type/out-of-reach.hpp"
#include <new>

namespace yack
{

    namespace chemical
    {
        actors:: ~actors() throw()
        {
            out_of_reach::zset(wksp,sizeof(wksp));
        }

        actors:: actors() throw() : wksp()
        {
        }

        double actors:: mass_action(double                  K,
                                    const readable<double> &C) const throw()
        {

            for(const actor *a=head;a;a=a->next)
            {
                const size_t   nu = a->nu; assert(nu>0);
                K *= ipower<double,size_t>( (**a)(C),nu);
            }
            return K;
        }


        const limiting * actors:: make(const actor *pa, const double xi) const throw()
        {
            assert(pa);
            assert(xi>=0);
            return new ( out_of_reach::zset(wksp,sizeof(wksp)) ) limiting(pa,xi);
        }


        const limiting *actors:: find_limiting(const readable<double> &C ) const throw()
        {
            switch( size )
            {
                case 0:  return NULL;
                case 1:  return make(head,head->extent(C));
                default: break;
            }

            assert(size>1);
            const actor   *pb = head;
            double         xi = pb->extent(C);
            for(const actor *pa=pb->next;pa;pa=pa->next)
            {
                const double   xi_temp = pa->extent(C);
                if(xi_temp<xi)
                {
                    pb = pa;
                    xi = xi_temp;
                }
            }
            return make(pb,xi);
        }

    }

}

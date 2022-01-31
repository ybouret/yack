
#include "yack/chem/actors.hpp"
#include "yack/arith/ipower.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/utils.hpp"
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
                K *= a->action(C);
            }
            return K;
        }


        void actors:: drvs_action(writable<double>       &psi,
                                  const double            K,
                                  const readable<double> &C) const throw()
        {

            assert(psi.size()<=C.size());

            for(const actor *a=head;a;a=a->next)
            {
                double res = a->derivs(K,C);
                for(const actor *b=a->prev;b;b=b->prev) res *= b->action(C);
                for(const actor *b=a->next;b;b=b->next) res *= b->action(C);
                (**a)(psi) = res;
            }
            
        }




        const limiting * actors:: make(const actor *pa, const double xi) const throw()
        {
            assert(pa);
            assert(xi>=0);
            return new ( YACK_STATIC_ZSET(wksp) ) limiting(pa,xi);
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

        double actors:: maximum(const readable<double> &C) const throw()
        {
            switch(size)
            {
                case 0: return 0;
                case 1: return (**head)(C);
                default:
                    break;
            }

            const actor *a = head;
            double       m = (**a)(C);
            for(a=a->next;a;a=a->next)
            {
                m = max_of(m, (**a)(C) );
            }
            return m;
        }

    }

}

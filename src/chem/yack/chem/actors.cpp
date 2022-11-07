

#include "yack/chem/actors.hpp"
#include "yack/type/utils.hpp"

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
                const double j = ***a;   assert(C[j]>=0);
                ops.upower(C[j],a->nu);
            }
            return ops.query();
        }

        double actors:: mass_action(const readable<double> &C,
                                    const double            xi,
                                    rmulops                &ops) const
        {
            assert(ops.size()==0||ops.size()==1);
            for(const actor *a=crew.head;a;a=a->next)
            {
                const double j  = ***a;           assert(C[j]>=0);
                ops.upower(max_of(C[j]+(a->nu)*xi,0.0),a->nu);
            }
            return ops.query();
        }
        
        void actors:: grad_action(writable<double>       &psi,
                                  const double            factor,
                                  const readable<double> &C,
                                  rmulops                &xmul) const
        {
            for(const actor *a=crew.head;a;a=a->next)
            {
                xmul            = factor;
                const double j  = ***a;            
                xmul.ld(a->nu);
                xmul.upower(C[j],a->nm);
                for(const actor *b=a->prev;b;b=b->prev) xmul.ld(C[***b]);
                for(const actor *b=a->next;b;b=b->next) xmul.ld(C[***b]);
                psi[j] = xmul.query();
            }
        }
        

        void actors:: move(writable<double> &C, const double xi) const throw()
        {
            for(const actor *a=crew.head;a;a=a->next)
            {
                const double j  = ***a;           assert(C[j]>=0);
                C[j] = max_of(C[j]+(a->nu)*xi,0.0);
            }
        }


        bool actors:: are_genuinely_blocked_by(const readable<double> &C) const throw()
        {
            for(const actor *a=crew.head;a;a=a->next)
            {
                if( C[***a] <= 0) return true;
            }
            return false;
        }

        bool actors:: are_primarily_blocked_by(const readable<double> &C) const throw()
        {
            for(const actor *a=crew.head;a;a=a->next)
            {
                if(!a->is_primary()) continue;
                if( C[***a] <= 0) return true;
            }
            return false;
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

        const xlimit *actors:: primary_limit(const readable<double> &C) const throw()
        {
            const actor *a  = crew.head;

            if(a)
            {
                // look for first primary
                while(!a->is_primary())
                {
                    a = a->next;
                    if(!a) return NULL;
                }
                assert(NULL!=a);

                // initialize
                const actor *id = a;
                double       xi = C[***a]/a->nu;

                // look for smaller primary
                for(a=a->next;a;a=a->next)
                {
                    if(!a->is_primary()) continue;;
                    const double xi_tmp = C[***a]/a->nu;
                    if(xi_tmp<xi)
                    {
                        xi = xi_tmp;
                        id = a;
                    }
                }

                // return limit
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

        const actor * actors:: unbalanced_primary(const readable<double> &C, double &xi) const throw()
        {
            // initialize search
            const actor *ua = NULL;
            xi              = 0;

            // loop over primaries
            for(const actor *a = crew.head;NULL!=a;a=a->next)
            {
                if(! a->is_primary() ) continue;
                const double c = C[ ***a ];
                if(c<0)
                {
                    const double x = -c/a->nu; assert(x>=0);
                    if(x>xi)
                    {
                        xi = x;
                        ua = a;
                    }
                }
            }
            return ua;
        }



    }

}


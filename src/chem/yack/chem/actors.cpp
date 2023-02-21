#include "yack/chem/actors.hpp"
#include "yack/type/utils.hpp"
#include <new>

namespace yack
{
    namespace chemical
    {

        actors:: ~actors() noexcept {}


        actors:: actors() noexcept :
        molecularity(0),
        algebraic_Z(0),
        crew()
        {

        }

        const list_of<actor> * actors:: operator->() const noexcept
        {
            const list_of<actor> &self = crew;
            return &self;
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
            coerce(crew).push_back( new actor(sp,nu) );
            coerce(molecularity) += nu;
            coerce(algebraic_Z) += int(nu)*sp.z;
        }

        void actors:: remove_last() noexcept
        {
            assert(crew.size);
            const actor   *last = crew.tail;
            const unsigned nu   = last->nu;
            const int      z    = (**last).z;

            coerce(molecularity) -= nu;
            coerce(algebraic_Z)  -= int(nu)*z;
            
            delete coerce(crew).pop_back();
        }

        bool actors:: attached_to(const actor &lhs) const noexcept
        {
            const size_t j = **lhs;
            for(const actor *rhs=crew.head;rhs;rhs=rhs->next)
            {
                if( j == ***rhs) return true;
            }
            return false;
        }

        bool actors:: attached_to(const actors &other) const noexcept
        {
            for(const actor *lhs=other->head;lhs;lhs=lhs->next)
            {
                if( attached_to(*lhs) ) return true;
            }
            return false;
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
        
        greatest actors:: grad_action(writable<double>       &psi,
                                      const double            factor,
                                      const readable<double> &C,
                                      rmulops                &xmul) const
        {
            greatest res;
            for(const actor *a=crew.head;a;a=a->next)
            {
                xmul            = factor; assert(1==xmul.size());
                const double j  = ***a;
                xmul.ld(a->nu);
                xmul.upower(C[j],a->nm);
                for(const actor *b=a->prev;b;b=b->prev) xmul.upower(C[***b],b->nu);
                for(const actor *b=a->next;b;b=b->next) xmul.upower(C[***b],b->nu);
                const greatest tmp( fabs( psi[j] = xmul.query() ), j);
                res << tmp;
            }
            return res;
        }
        

        void actors:: move(writable<double> &C, const double xi) const noexcept
        {
            for(const actor *a=crew.head;a;a=a->next)
            {
                const double j  = ***a;           assert(C[j]>=0);
                C[j] = max_of(C[j]+(a->nu)*xi,0.0);
            }
        }

#if 0
        void  actors:: mov_(writable<double> &C, const double xi) const noexcept
        {
            for(const actor *a=crew.head;a;a=a->next)
            {
                const double j  = ***a;
                C[j] =   C[j]+(a->nu)*xi;
            }
        }
#endif

        bool actors:: are_genuinely_blocked_by(const readable<double> &C) const noexcept
        {
            for(const actor *a=crew.head;a;a=a->next)
            {
                if( C[***a] <= 0) return true;
            }
            return false;
        }

#if 0
        bool actors:: are_primarily_blocked_by(const readable<double> &C) const noexcept
        {
            for(const actor *a=crew.head;a;a=a->next)
            {
                if(!a->is_primary()) continue;
                if( C[***a] <= 0) return true;
            }
            return false;
        }
#endif


        

        const xlimit     * actors:: genuine_limit(xlimit::field &xl, const readable<double> &C) const noexcept
        {
            assert(xl.num_bytes>=sizeof(xlimit));
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
                return new (*xl) xlimit(*id,xi);
            }
            else
            {
                return NULL;
            }
        }

#if 0
        const xlimit *actors:: primary_limit(const readable<double> &C) const noexcept
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
                return new ( *wlim ) xlimit(*id,xi);
            }
            else
            {
                return NULL;
            }

        }
#endif

        
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


        string actors :: to_string() const
        {
            string res;
            const actor *a = crew.head;
            if(a)
            {
                res = a->to_first();
                for(a=a->next;a;a=a->next)
                {
                    res += a->to_extra();
                }
            }
            else
            {
                res += '.';
            }
            return res;
        }


        void actors:: display_compact(std::ostream &os, const readable<double> &C) const
        {
            for(const actor *a = crew.head;a;a=a->next)
            {
                a->display_compact(os << ' ',C);
            }
            
        }

        void actors:: display_compact(std::ostream &os, const readable<double> &C, const readable<bool> &flag) const
        {
            for(const actor *a = crew.head;a;a=a->next)
            {
                const species &s = **a;
                if(flag[*s])
                    a->display_compact(os << ' ',C);
            }

        }


        
    }

}


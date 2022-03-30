
#include "yack/chem/actors.hpp"
#include "yack/arith/ipower.hpp"

namespace yack
{
    namespace chemical
    {
        actors:: ~actors() throw()
        {
        }

        actors:: actors() throw() :
        actors_(),
        wksp()
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

        void actors:: drvs_action(writable<double>       &psi,
                                  const double            factor,
                                  const readable<double> &C,
                                  writable<double>       &arr) const throw()
        {
            for(const actor *a=head;a;a=a->next)
            {
                arr[ ***a ] = a->mass_action(C);
            }

            for(const actor *a=head;a;a=a->next)
            {
                double value = factor * a->drvs_action(C);
                for(const actor *b=a->prev;b;b=b->prev) value *= arr[ ***b ];
                for(const actor *b=a->next;b;b=b->next) value *= arr[ ***b ];
                psi[ ***a ] = value;
            }
        }

        double actors:: grad_action(writable<double>       &psi,
                                    const double            factor,
                                    const readable<double> &C,
                                    writable<double>       &arr) const throw()
        {
            double ma = factor;
            for(const actor *a=head;a;a=a->next)
            {
                ma *= ( arr[ ***a ] = a->mass_action(C) );
            }

            for(const actor *a=head;a;a=a->next)
            {
                double value = factor * a->drvs_action(C);
                for(const actor *b=a->prev;b;b=b->prev) value *= arr[ ***b ];
                for(const actor *b=a->next;b;b=b->next) value *= arr[ ***b ];
                psi[ ***a ] = value;
            }


            return ma;
        }


        void actors:: hessian(matrix<double>         &H,
                              const double            factor,
                              const readable<double> &C,
                              writable<double>       &arr) const throw()
        {

            for(const actor *a=head;a;a=a->next)
            {
                arr[ ***a ] = a->mass_action(C);
            }

            for(const actor *ak=head;ak;ak=ak->next)
            {
                const unsigned nu_k = ak->coef;
                const unsigned k    = ***ak;
                if(nu_k>1)
                {
                    double value = factor * nu_k * (nu_k-1) * ipower(C[k],nu_k-2);
                    for(const actor *b=ak->prev;b;b=b->prev) value *= arr[ ***b ];
                    for(const actor *b=ak->next;b;b=b->next) value *= arr[ ***b ];
                    H[k][k] = value;
                }

                for(const actor *al=ak->next;al;al=al->next)
                {
                    const unsigned nu_l = al->coef;
                    const unsigned l    = ***al;
                    double value = factor * nu_k * ipower(C[k],nu_k-1) * nu_l * ipower(C[l],nu_l-1);
                    for(const actor *b=head;b;b=b->next)
                    {
                        if(b!=ak && b!=al) value *= arr[ ***b ];
                    }
                    H[k][l] = H[l][k] = value;
                }

            }
        }


        double actors:: mass_action(double factor, const readable<double> &C, const double xi) const throw()
        {
            for(const actor *a=head;a;a=a->next)
            {
                factor *= a->mass_action(C,xi);
            }
            return factor;
        }

        double actors:: slope(const readable<double> &C, const double xi) const throw()
        {
            double sum = 0;
            for(const actor *a=head;a;a=a->next)
            {
                double p = a->slope(C,xi);
                for(const actor *b=a->prev;b;b=b->prev) p *= b->mass_action(C,xi);
                for(const actor *b=a->next;b;b=b->next) p *= b->mass_action(C,xi);
                sum += p;
            }
            return sum;
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

        void actors:: move(writable<double> &C, const double xi) const throw()
        {
            for(const actor *a=head;a;a=a->next)
            {
                a->move(C,xi);
            }
        }

        double actors:: maximum(const readable<double> &C) const throw()
        {
            double ans = 0;
            for(const actor *a=head;a;a=a->next)
            {
                const double c = C[***a]; assert(c>=0);
                if(c>ans) ans=c;
            }
            return ans;
        }

        void actors:: update_last()
        {
            assert(tail);
            actor *a = tail;
            while(a->prev && (a->prev->coef>a->coef) ) towards_front(a);
        }

    }

}

#include "yack/type/out-of-reach.hpp"

namespace yack
{
    namespace chemical
    {
        const limit * actors:: private_limit(const readable<double> &C) const throw()
        {
            const actor *best = head;
            if(best)
            {
                double   mini = best->limiting_extent(C); assert(mini>=0);
                for(const actor *curr=best->next;curr;curr=curr->next)
                {
                    const double temp = curr->limiting_extent(C);
                    if(temp<mini)
                    {
                        mini = temp;
                        best = curr;
                    }

                }

                return new (YACK_STATIC_ZSET(wksp)) limit(*best,mini);
            }
            else
            {
                return NULL;
            }
        }

        const limit * actors:: primary_limit(const readable<double> &C) const throw()
        {
            const actor *best = head;
            while(best)
            {
                if( (**best).rank == 1) goto FOUND;
                best = best->next;
            }
            return NULL;

        FOUND:
            assert(best);

            double mini = best->limiting_extent(C); assert(mini>=0);
            for(const actor *curr=best->next;curr;curr=curr->next)
            {
                if(1!=(**curr).rank) continue;;
                const double temp = curr->limiting_extent(C);
                if(temp<mini)
                {
                    mini = temp;
                    best = curr;
                }
            }

            return new (YACK_STATIC_ZSET(wksp)) limit(*best,mini);
        }


    }

}


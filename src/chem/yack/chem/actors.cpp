
#include "yack/chem/actors.hpp"

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

        void actors:: drvs_action(writable<double> &psi, const double factor, const readable<double> &C) const throw()
        {

            for(const actor *a=head;a;a=a->next)
            {
                double value = factor * a->drvs_action(C);
                for(const actor *b=a->prev;b;b=b->prev) value *= b->mass_action(C);
                for(const actor *b=a->next;b;b=b->next) value *= b->mass_action(C);
                psi[ ***a ] = value;
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
                const double c = (**a)[C]; assert(c>=0);
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


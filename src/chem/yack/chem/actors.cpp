
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

        double actors:: mass_action(double factor, const readable<double> &C, const double xi) const throw()
        {
            for(const actor *a=head;a;a=a->next)
            {
                factor *= a->mass_action(C,xi);
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


    }

}


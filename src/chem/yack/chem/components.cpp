#include "yack/chem/components.hpp"
#include <iomanip>
#include <cmath>

namespace yack
{
    namespace chemical
    {

        components:: components() throw() :
        reac(),
        prod(),
        d_nu(0),
        sexp(0),
        nu_p(0),
        nu_r(0),
        topo(),
        db(),
        wksp()
        {

        }

        components:: ~components() throw()
        {
            for(const cnode *node=head();node;node=node->next)
            {
                const species &s = ****node;
                --coerce(s.rank);
            }
        }

        const cnode * components:: head() const throw()
        {
            return (*db.tree).head;
        }

        size_t components:: size() const throw()
        {
            return db.size();
        }

        const component & components:: operator[](const size_t k) const throw()
        {
            assert(k>0);
            assert(k<=size());
            const cnode *node = (*db.tree).get(k);
            return ***node;
        }

        void components:: update() throw()
        {
            coerce(d_nu) = (coerce(nu_p)=prod.nu()) - (coerce(nu_r)=reac.nu());
            if(d_nu!=0)
            {
                coerce(sexp) = 1.0/d_nu;
            }
            else
            {
                coerce(sexp) = 1.0;
            }
        }


        static inline
        void actors_to_string( string &topo, const actors &a)
        {
            const actor *node = a.head;
            if(node)
            {
                topo += vformat("%u",node->coef);
                for(node=node->next;node;node=node->next)
                {
                    topo += ',';
                    topo += vformat("%u",node->coef);
                }
            }
        }

        void components:: mktopo()
        {
            string res;
            actors_to_string(res,reac);
            res += ':';
            actors_to_string(res,prod);

            coerce(topo) = res;
        }




        bool components:: operator()(const species &sp,
                                     const int      nu)
        {
            assert(nu!=0);
            const component::pointer p = new component(sp,nu);
            if(!db.insert(p)) return false;

            try
            {
                if(nu>0)
                {
                    coerce(prod).push_back( new actor(sp,nu) );
                }
                else
                {
                    assert(nu<0);
                    coerce(reac).push_back( new actor(sp,-nu) );
                }
                mktopo();
            }
            catch(...)
            {
                db.remove(sp.name);
                throw;
            }


            update();
            ++coerce(sp.rank);


            return true;
        }

        double components:: mass_action(const double K, const readable<double> &C) const throw()
        {
            return reac.mass_action(K,C) - prod.mass_action(1.0,C);
        }

        double components:: mass_action(const double K, const readable<double> &C, const double xi) const throw()
        {
            return reac.mass_action(K,C,-xi) - prod.mass_action(1.0,C,xi);
        }

        double components:: slope(const double K, const readable<double> &C, const double xi) const throw()
        {
            return -(K*reac.slope(C,-xi)+prod.slope(C,xi));
        }
        
        void components:: move(writable<double> &C, const double xi) const throw()
        {
            reac.move(C,-xi);
            prod.move(C,+xi);
        }


        void components:: drvs_action(writable<double> &psi, const double K, const readable<double> &C) const throw()
        {
            reac.drvs_action(psi, K,C);
            prod.drvs_action(psi,-1,C);
        }

        double components:: Q(const readable<double> &C) const throw()
        {
            assert(are_valid(C));
            const double den = reac.mass_action(1,C);
            return fabs(den) > 0 ? prod.mass_action(1,C)/den : 0;
        }


        std::ostream &components:: display(std::ostream &os) const
        {
            reac.display(os);
            os << " <=> ";
            prod.display(os);
            //os << " | d_nu=" << d_nu << " | nu_p=" << nu_p;
            //os << " {" << topo << "}";
            return os;
        }

        const limits & components:: private_limits(const readable<double> &C) const throw()
        {
            return * new( YACK_STATIC_ZSET(wksp) ) limits( reac.private_limit(C), prod.private_limit(C) );
        }

        const limits & components:: primary_limits(const readable<double> &C) const throw()
        {
            return * new( YACK_STATIC_ZSET(wksp) ) limits( reac.primary_limit(C), prod.primary_limit(C) );
        }


        bool components:: is_neutral() const throw()
        {
            return 0== (reac.dz() - prod.dz());
        }

        bool components:: are_valid(const readable<double> &C) const throw()
        {
            for(const cnode *node=head();node;node=node->next)
            {
                const species &s = ****node;
                if( C[*s] < 0) return false;
            }
            return true;
        }

        double components:: deduce(const readable<double> &C0,
                                   const readable<double> &Cs) const throw()
        {
            double sum = 0;
            for(const cnode *node=head();node;node=node->next)
            {
                const component &c = ***node;
                const int        n = c.coef;
                const species   &s = *c;
                const size_t     j = *s;
                const double    dC = Cs[j] - C0[j];
                const double    xi = dC/n;
                sum += xi;
            }
            return  sum/size();
        }


    }

}

#include "yack/arith/gcd.h"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace chemical
    {
        double components:: maximum(const readable<double> &C) const throw()
        {
            return max_of( reac.maximum(C), prod.maximum(C) );
        }


        bool components:: is_minimal() const throw()
        {
            if(size()>=2)
            {
                const cnode *curr = head();     assert(curr);
                const cnode *next = curr->next; assert(next);
                uint64_t     g    = yack_gcd64( absolute( (***curr).coef ), absolute( (***next).coef ) );
                for(curr=next->next;curr;curr=curr->next)
                {
                    g = yack_gcd64(g, absolute( (***curr).coef ));
                }
                return 1 == g;
            }
            return true;
        }

    }
}


#include "yack/chem/components.hpp"

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
            }
            catch(...)
            {
                db.remove(sp.name);
                throw;
            }

            ++coerce(sp.rank);

            update();

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
            prod.move(C,xi);
        }



        std::ostream &components:: display(std::ostream &os) const
        {
            reac.display(os);
            os << " <=> ";
            prod.display(os);
            os << " | d_nu=" << d_nu << " | nu_p=" << nu_p;
            return os;
        }

        const limits & components:: private_limits(const readable<double> &C) const throw()
        {
            return * new( YACK_STATIC_ZSET(wksp) ) limits( reac.private_limit(C), prod.private_limit(C) );
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
                if( s[C] < 0) return false;
            }
            return true;
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

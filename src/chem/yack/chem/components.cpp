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


        bool components:: detached(const components &other) const throw()
        {
            for(const cnode *lhs=head();lhs;lhs=lhs->next)
            {
                const size_t j = *****lhs;
                for(const cnode *rhs=other.head();rhs;rhs=rhs->next)
                {
                    const size_t k = *****rhs;
                    if(j==k) return false;
                }
            }
            return true; // no common => detached
        }

        bool components:: attached(const components &other) const throw()
        {
            for(const cnode *lhs=head();lhs;lhs=lhs->next)
            {
                const size_t j = *****lhs;
                for(const cnode *rhs=other.head();rhs;rhs=rhs->next)
                {
                    const size_t k = *****rhs;
                    if(j==k)
                    {
                        return true;
                    }
                }
            }
            return false; // no common => not attached
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

        bool components:: found_underflow_for(const double xi, const readable<double> &C) const throw()
        {
            for(const cnode *node=head();node;node=node->next)
            {
                const component &c = ***node;
                if(c.is_too_small(xi,C)) return true;
            }
            return false;
        }


#if 0
        bool components:: extent_changes_phase_space(const readable<double> &C, const double xi, writable<double> &Ctry) const throw()
        {
            for(const cnode *node=head();node;node=node->next)
            {
                const size_t j = *****node;
                Ctry[j] = C[j];
            }

            move(Ctry,xi);

            for(const cnode *node=head();node;node=node->next)
            {
                const size_t j = *****node;
                if( fabs(Ctry[j]-C[j]) > 0) return true;
            }

            return false;
        }

        bool components:: extent_changes_mass_action(const double K, const readable<double> &C, const double xi, writable<double> &Ctry) const throw()
        {
            const double maOrg = mass_action(K,C);
            for(const cnode *node=head();node;node=node->next)
            {
                const size_t j = *****node;
                Ctry[j] = C[j];
            }

            move(Ctry,xi);
            const double maTry = mass_action(K,Ctry);

            return fabs(maOrg-maTry)>0;
        }
#endif


        void components:: hessian(matrix<double> &H, const double K, const readable<double> &C, writable<double> &arr) const throw()
        {
            H.ld(0);
            reac.hessian(H, K,C,arr);
            prod.hessian(H,-1,C,arr);
        }


        void components:: drvs_action(writable<double>       &psi,
                                      const double            K,
                                      const readable<double> &C,
                                      writable<double>       &arr) const throw()
        {
            psi.ld(0);
            reac.drvs_action(psi, K,C,arr);
            prod.drvs_action(psi,-1,C,arr);
        }

        double components:: grad_action(writable<double> &psi, const double K, const readable<double> &C, writable<double> &arr) const throw()
        {
            psi.ld(0);
            return reac.grad_action(psi,K,C,arr) + prod.grad_action(psi,-1,C,arr);
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

        std::ostream & components:: display_signature(std::ostream &os) const
        {
            os << '[';
            const cnode *node = head();
            if(node)
            {
                os << (*****node);
                for(node=node->next;node;node=node->next)
                {
                    os << ';';
                    os << (*****node);
                }
            }
            os << ']';
            return os;
        }

        bool components:: found_species_index(const size_t i) const throw()
        {
            for(const cnode *node=head();node;node=node->next)
            {
                const size_t j = *****node;
                if(i==j) return true;
            }
            return false;
        }


        bool components:: other_are_unchanged(const readable<double> &lhs,
                                              const readable<double> &rhs) const throw()
        {
            assert(lhs.size()==rhs.size());
            for(size_t j=lhs.size();j>0;--j)
            {
                if(found_species_index(j)) continue;
                if( fabs( lhs[j] - rhs[j] ) > 0 )
                {
                    return false;
                }
            }
            return true;
        }

        const limits & components:: private_limits(const readable<double> &C, const size_t w) const throw()
        {
            return * new( YACK_STATIC_ZSET(wksp) ) limits( reac.private_limit(C), prod.private_limit(C), w);
        }

        const limits & components:: primary_limits(const readable<double> &C, const size_t w) const throw()
        {
            return * new( YACK_STATIC_ZSET(wksp) ) limits( reac.primary_limit(C), prod.primary_limit(C), w);
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
            double sum       = 0;
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
            return sum/size();
        }


    }

}

#include "yack/arith/gcd.h"
#include "yack/type/utils.hpp"
#include "yack/exception.hpp"

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


        void components:: must_comply(const string &name) const
        {
            if( !is_neutral() ) throw exception("%s is not neutral",name());
            if( !is_minimal() ) throw exception("%s is not minimal",name());
        }
    }
}

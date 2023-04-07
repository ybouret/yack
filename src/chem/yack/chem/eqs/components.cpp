
#include "yack/chem/eqs/components.hpp"
#include "yack/system/imported.hpp"
#include "yack/type/abs.hpp"
#include "yack/arith/gcd.h"

namespace yack
{
    namespace chemical
    {
        components:: ~components() noexcept
        {
        }

        components:: components() noexcept :
        large_object(),
        counted(),
        reac(),
        prod(),
        d_nu(0),
        idnu(0),
        cdb()
        {
        }

        components:: components(const components &other) :
        large_object(),
        counted(),
        reac( other.reac ),
        prod( other.prod ),
        d_nu( other.d_nu ),
        idnu( other.idnu ),
        cdb(  other.cdb  )
        {

        }

        const components_db::tree_type & components:: operator->() const noexcept
        {
            return cdb.tree;
        }



        const char * const components::clid = "components";

        void components:: operator()(const int nu, const species &sp)
        {
            assert(nu!=0);
            if( cdb.search(sp.name) ) throw imported::exception(clid,"multiple [%s]", sp.name());
            const component::ptr cm = new component(sp,nu);
            actors              *ac = 0;
            if(nu>0)
            {

                (ac = &coerce(prod))->push_back(new actor(sp,static_cast<unsigned>(nu)));
            }
            else
            {
                (ac = &coerce(reac))->push_back(new actor(sp,static_cast<unsigned>(-nu)));
            }

            try
            {
                if(!cdb.insert(cm)) throw imported::exception(clid,"failure to insert %d[%s]",nu,sp.name());
            }
            catch(...)
            {
                delete ac->pop_back();
                throw;
            }

            // update
            coerce(d_nu) += nu;
            coerce(idnu) = d_nu !=0 ? 1.0/d_nu : 0;

        }

        static const char rightleft[] = " <=> ";

        std::ostream & operator<<(std::ostream &os, const components &self)
        {
            os << self.reac << rightleft << self.prod;
            return os;
        }

        string components:: to_string() const
        {
            return reac.to_string() + rightleft + prod.to_string();
        }


        double components:: mass_action(const index_level       I,
                                        const readable<double> &C,
                                        const double            K,
                                        cameo::mul<double>     &xmul) const
        {
            assert(K>0);
            xmul.free();
            xmul.push(K);
            reac.mass_action(I,C,xmul);
            const double lhs = xmul.product();

            assert(0==xmul.size());
            xmul.push(1);
            prod.mass_action(I,C,xmul);
            return lhs - xmul.product();
        }

        double components:: mass_action(const index_level       I,
                                        const readable<double> &C,
                                        const double            K,
                                        const double            xi,
                                        cameo::mul<double>     &xmul) const
        {
            assert(K>0);
            xmul.free();
            xmul.push(K); assert(1==xmul.size());
            reac.mass_action(I,C,-xi,xmul);
            const double lhs = xmul.product();

            assert(0==xmul.size());
            xmul.push(1); assert(1==xmul.size());
            prod.mass_action(I,C,xi,xmul);
            const double rhs = xmul.product();
            return lhs - rhs;
        }

        double components:: mass_action(const index_level       I,
                                        const double            u,
                                        const readable<double> &C0,
                                        const readable<double> &C1,
                                        const double           &K,
                                        writable<double>       &Ctmp,
                                        cameo::mul<double>     &xmul) const
        {
            if(u<=0)
            {
                return mass_action(I,C0,K,xmul);
            }
            else
            {
                if(u>=1)
                {
                    return mass_action(I,C1,K,xmul);
                }
                else
                {
                    const double v = 1.0-u;
                    for(const cnode *cn=(*this)->head;cn;cn=cn->next)
                    {
                        const size_t j    = (****cn).indx[I];
                        const double c0   = C0[j];
                        const double c1   = C1[j];
                        double       cmin = c0;
                        double       cmax = c1;
                        if(cmax<cmin) cswap(cmin,cmax);
                        Ctmp[j] = clamp(cmin,c1*u+c0*v,cmax);
                    }
                    return mass_action(I,Ctmp,K,xmul);
                }
            }
        }



        void components:: move(const index_level I, writable<double> &C, const double xi) const noexcept
        {
            for(const cnode *node=(*this)->head;node;node=node->next)
            {
                const component &cc = ***node;
                const species   &sp = *cc;
                const size_t     i  = sp.indx[I];
                C[i] = max_of( C[i]+(cc.nu*xi), 0.0);
            }
        }

        void components:: mov_(const index_level I,
                               writable<double> &C,
                               const double      xi) const noexcept
        {
            for(const cnode *node=(*this)->head;node;node=node->next)
            {
                const component &cc = ***node;
                const species   &sp = *cc;
                const size_t     i  = sp.indx[I];
                C[i] += (cc.nu*xi);
            }
        }


        static inline double expand_grad(const actor            *a,
                                         const readable<double> &C,
                                         const index_level       I,
                                         cameo::mul<double>     &xmul)
        {
            for(const actor *b=a->prev;b;b=b->prev)  b->mass_action(I,C,xmul);
            for(const actor *b=a->next;b;b=b->next)  b->mass_action(I,C,xmul);
            return xmul.product();
        }

        static inline void   update_grad(writable<double>       &psi,
                                         const actor            *a,
                                         const readable<double> &C,
                                         const index_level       I,
                                         cameo::mul<double>     &xmul)
        {
            const size_t i = a->grad_action(I,C,xmul);
            psi[i] = expand_grad(a,C,I,xmul);
        }

        void components:: grad(const index_level       I,
                               writable<double>       &psi,
                               const readable<double> &C,
                               const double            K,
                               cameo::mul<double>     &xmul) const
        {
            psi.ld(0);

            for(const actor *a=reac.head;a;a=a->next)
            {
                xmul = K;
                update_grad(psi,a,C,I,xmul);
            }

            for(const actor *a=prod.head;a;a=a->next)
            {
                xmul = -1;
                update_grad(psi,a,C,I,xmul);
            }

        }

        static inline
        void update_hess(const double            f,
                         matrix<double>         &H,
                         const actor            *a,
                         const readable<double> &C,
                         const index_level       I,
                         cameo::mul<double>      &xmul)
        {
            const species &A_k = **a;
            const size_t   k    = A_k.indx[I];
            if(a->nu>=2)
            {
                xmul.free();
                xmul.push(f);
                xmul.push(a->nu);
                xmul.push(a->nm);
                xmul.push(C[k],a->nd);
                H[k][k] = expand_grad(a,C,I,xmul);
            }

            for(const actor *b=a->next;b;b=b->next)
            {
                const species &A_l = **b;
                const size_t   l   = A_l.indx[I];
                xmul.free();
                xmul.push(f);

                xmul.push(a->nu);
                xmul.push(C[k],a->nm);

                xmul.push(b->nu);
                xmul.push(C[l],b->nm);

                for(const actor *d=a->next;d;d=d->next)
                {
                    if(d==b) continue;
                    xmul.push(C[(**d).indx[I]],d->nu);
                }
                H[k][l] = H[l][k] = xmul.product();
            }
        }

        void components:: hessian(const index_level       I,
                                  matrix<double>         &H,
                                  const readable<double> &C,
                                  const double            K,
                                  cameo::mul<double>     &xmul) const
        {
            H.ld(0);
            for(const actor *a=reac.head;a;a=a->next)
            {
                update_hess(K,H,a,C,I,xmul);
            }

            for(const actor *a=prod.head;a;a=a->next)
            {
                update_hess(-1,H,a,C,I,xmul);
            }
        }


        double components:: slope(const index_level       I,
                                  const readable<double> &C,
                                  const double            K,
                                  cameo::mul<double>     &xmul,
                                  cameo::add<double>     &xadd) const
        {
            xadd.free();
            for(const actor *a=reac.head;a;a=a->next)
            {
                xmul = K;
                (void) a->grad_action(I,C,xmul);
                xadd.push( (a->nu*expand_grad(a,C,I,xmul)) );
            }

            for(const actor *a=prod.head;a;a=a->next)
            {
                xmul = 1;
                (void) a->grad_action(I,C,xmul);
                xadd.push( (a->nu*expand_grad(a,C,I,xmul)) );
            }

            return -xadd.sum();
        }

        double components:: quotient(const index_level       I,
                                     const readable<double> &C,
                                     const double            K,
                                     cameo::mul<double>     &xmul) const
        {
            assert(K>0);
            xmul.free();
            for(const cnode *node=(*this)->head;node;node=node->next)
            {
                const component &cc = ***node;
                const species   &sp = *cc;
                const size_t     ii = sp.indx[I];
                const double     c  = C[ii];
                if(c<=0) return 0;
                xmul.ipower(c,cc.nu);
            }
            xmul.push(1.0/K);
            return xmul.product();
        }


        bool components:: is_neutral() const noexcept
        {
            int z = 0;
            for(const cnode *node= (*this)->head; node; node=node->next)
            {
                const component &cc = ***node;
                const species   &sp = *cc;
                const int        nu = cc.nu;
                z += nu * sp.z;
            }
            return z==0;
        }

        bool components:: is_minimal() const noexcept
        {
            const cnode *node= (*this)->head;
            if(node)
            {
                uint64_t g = absolute( (***node).nu );
                for(node=node->next;node;node=node->next)
                {
                    g = yack_gcd64(g, absolute( (***node).nu ));
                }
                return 1 == g;
            }
            else
            {
                // no node
                return true;
            }
        }

        bool components:: is_connected_to(const species &sp) const noexcept
        {
            return NULL != cdb.search(sp.name);
        }

        bool components:: is_connected_to(const components &other) const noexcept
        {
            for(const cnode *node=other->head;node;node=node->next)
            {
                if( is_connected_to(****node) ) return true;
            }
            return false;
        }

        void components:: report_to(addrbook &db) const
        {
            for(const cnode *node=(*this)->head;node;node=node->next)
            {
                const species &s = ****node;
                db.ensure(&s);
            }
        }

        



    }
}



#include "yack/chem/eqs/components.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/integer.hpp"
#include "yack/type/abs.hpp"

namespace yack
{
    namespace Chemical
    {
        Components:: Components() noexcept :
        reac(),
        prod(),
        d_nu(0),
        sexp(0),
        cdb()
        {
        }

        Components:: ~Components() noexcept
        {
        }


        const cList * Components:: operator->() const throw()
        {
            return & *cdb.get_tree();
        }

        const char Components:: CLID[] = "Chemical::Components";

        void Components:: operator()(const int nu, const Species &sp)
        {
            assert(nu!=0);
            const Component::Pointer ptr = new Component(nu,sp);
            if(!cdb.insert(ptr))
                throw imported::exception(CLID,"multiple component '%s'", sp.name() );

            Actors  *A = NULL;
            try
            {
                unsigned n = 0;
                if(nu>0) {
                    A = & coerce(prod);
                    n = static_cast<unsigned>(nu);
                }
                else
                {
                    A = & coerce(reac);
                    n = static_cast<unsigned>(-nu);
                }
                A->push_back( new Actor(n,sp) );
            }
            catch(...)
            {
                (void) cdb.remove(sp.name);
                throw;
            }
            assert(A);
            A->makeName();
            {
                int &sum = coerce(d_nu);
                sum  = 0;
                for(const cNode *node= (*this)->head;node;node=node->next)
                {
                    const Component &cc = ***node;
                    sum += cc.nu;
                }
            }
            if(d_nu!=0)
            {
                coerce(sexp) = 1.0/d_nu;
            }
            else
            {
                coerce(sexp) = 0.0;
            }
        }

        std::ostream & Components:: display(std::ostream &os) const
        {
            os << reac.name << " <=> " << prod.name;
            return os;
        }

        bool Components:: contains(const Species &sp) const noexcept
        {
            const Component::Pointer *ppc = cdb.search(sp.name);
            if(ppc)
            {
                assert(& ***ppc == &sp);
                return true;
            }
            else
            {
                return false;
            }
        }

        void Components:: submitTo(addrbook &ab) const
        {
            for(const cNode *cn = (*this)->head; cn; cn=cn->next)
            {
                const Species &sp = ****cn;
                ab.ensure(&sp);
            }
        }


        bool Components:: linkedTo(const Components &other) const noexcept
        {
            for(const cNode *node=other->head;node;node=node->next)
            {
                if(contains(****node)) return true;
            }
            return false;
        }

        bool Components:: neutral() const
        {
            apz dz = 0;
            for(const cNode *node=(*this)->head;node;node=node->next)
            {
                const Component &cc = ***node;
                const apz        nu = cc.nu;
                const apz        z  = (*cc).z;
                dz += z * nu;
            }
            return __zero__ == dz.s;
        }


        bool Components:: minimal() const
        {
            const cList &self = *cdb.get_tree();
            switch(self.size)
            {
                case 0:
                case 1:
                    return true;
            }
            const cNode *node = self.head;
            apn g = absolute( ( (***node).nu ) );
            if(1==g) return true;
            for(node=node->next;node;node=node->next)
            {
                const apn nu = absolute( (***node).nu );
                g = apn::gcd(g,nu); if(1==g) return true;
            }
            return false;

        }


        Extended::Real Components:: reacMassAction(Extended::Mul                  &xmul,
                                               const Extended::Real            K,
                                               const readable<Extended::Real> &C,
                                               const IndexLevel                level) const
        {
            xmul.free();
            xmul.insert(K);
            reac.massAction(xmul,C,level);
            return xmul.reduce();

        }


        Extended::Real Components:: prodMassAction(Extended::Mul                  &xmul,
                                                   const readable<Extended::Real> &C,
                                                   const IndexLevel                level) const
        {
            xmul.free();
            xmul.insert(xmul.positive1);
            prod.massAction(xmul,C,level);
            return xmul.reduce();
        }

        Extended::Real Components:: massAction(Extended::Mul                  &xmul,
                                               const Extended::Real            K,
                                               const readable<Extended::Real> &C,
                                               const IndexLevel                level) const
        {

            return reacMassAction(xmul,K,C,level) - prodMassAction(xmul,C,level);
        }

        Extended::Real Components:: quotient(Extended::Mul                  &xmul,
                                             const Extended::Real            K,
                                             const readable<Extended::Real> &C,
                                             const IndexLevel                level) const
        {
            return prodMassAction(xmul,C,level)/reacMassAction(xmul,K,C,level);
        }


        void Components:: gradAction(writable<Extended::Real>       &out,
                                     const IndexLevel                outLevel,
                                     const Extended::Real           &K,
                                     const readable<Extended::Real> &inp,
                                     const IndexLevel                inpLevel,
                                     Extended::Mul                  &xmul) const
        {
            reac.gradAction(out, outLevel, K,              inp, inpLevel, xmul);
            prod.gradAction(out, outLevel, xmul.negative1, inp, inpLevel, xmul);
        }


        Extended::Real Components:: dot(const readable<Extended::Real> & phi,
                                        const IndexLevel                 phiLevel,
                                        Extended::Add                  & xadd) const
        {
            xadd.resume((*this)->size);

            for(const cNode *node=(*this)->head;node;node=node->next)
            {
                const Component     &cc = ***node;
                const Species       &sp = *cc;
                const Extended::Real pr = phi[ sp.indx[phiLevel] ] * cc.xn;
                xadd.append(pr);
            }

            return xadd.reduce();
        }
    }

}

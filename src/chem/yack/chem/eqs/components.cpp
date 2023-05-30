
#include "yack/chem/eqs/components.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/integer.hpp"

namespace yack
{
    namespace Chemical
    {
        Components:: Components() noexcept :
        reac(),
        prod(),
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
            apn g = std::abs( (***node).nu ); if(1==g) return true;
            for(node=node->next;node;node=node->next)
            {
                const apn nu = std::abs( (***node).nu );
                g = apn::gcd(g,nu); if(1==g) return true;
            }
            return false;

        }
    }

}


#include "yack/chem/eqs/components.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace Chemical
    {
        Components:: Components() noexcept
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

        bool Components:: uses(const Species &sp) const noexcept
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


        bool Components:: connectedTo(const Components &other) const noexcept
        {
            for(const cNode *node=other->head;node;node=node->next)
            {
                if(uses(****node)) return true;
            }
            return false;
        }

    }

}

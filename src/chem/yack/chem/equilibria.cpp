
#include "yack/chem/equilibria.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace chemical
    {
        equilibria:: equilibria() throw() : compendium(), db()
        {
        }

        equilibria:: ~equilibria() throw()
        {
        }

        size_t equilibria:: size() const throw()
        {
            return db.size();
        }

        const enode * equilibria:: head() const throw()
        {
            return (*db.tree).head;
        }

        const equilibrium & equilibria:: operator[](const size_t indx) const throw()
        {
            return ***((*db.tree).get(indx));
        }


        equilibrium & equilibria:: use( equilibrium *eq )
        {
            assert(NULL!=eq);
            const equilibrium::pointer pEq(eq);
            if(!db.insert(pEq)) throw exception("multiple equilibrium '%s'", eq->name() );
            update_with(*eq);
            return *eq;
        }


        std::ostream & operator<<(std::ostream &os, const equilibria &eqs)
        {
            os << '{' << std::endl;
            for(const enode *node = eqs.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                eqs.pad(os << "  " << eq.name,eq) << " : ";
                eq.display(os) << " ( " << eq.K(equilibrium::display_time) << " )";
                os << std::endl;
            }
            os << '}';
            return os;
        }
    }

}


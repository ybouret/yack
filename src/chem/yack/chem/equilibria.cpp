
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

        equilibrium & equilibria:: use( equilibrium *eq )
        {
            assert(NULL!=eq);
            const equilibrium::pointer pEq(eq);
            if(!db.insert(pEq)) throw exception("multiple equilibrium '%s'", eq->name() );
            return *eq;
        }
    }

}



#include "yack/chem/equilibria.hpp"
#include "yack/exception.hpp"

namespace yack
{

    namespace chemical
    {

        equilibria:: ~equilibria() throw()
        {

        }

        equilibria:: equilibria() throw() : equilibria_()
        {

        }


        equilibrium & equilibria:: use(equilibrium *pEq)
        {
            assert(pEq);
            const equilibrium::pointer eq = pEq;
            if(!insert(eq)) throw exception("equilibria use multiple <%s>", (eq->name)());
            return *pEq;
        }

        const enode * equilibria:: head() const throw()
        {
            return (*tree).head;
        }


    }

}


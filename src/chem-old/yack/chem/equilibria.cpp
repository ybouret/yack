
#include "yack/chem/equilibria.hpp"
#include "yack/exception.hpp"
#include "yack/type/utils.hpp"

namespace yack
{

    namespace chemical
    {

        equilibria:: ~equilibria() throw()
        {

        }

        equilibria:: equilibria() throw() : compendium(), db()
        {

        }

        equilibrium & equilibria:: use(equilibrium *pEq)
        {
            assert(pEq);
            // try to insert equilibrium
            const equilibrium::pointer eq = pEq;
            if(!db.insert(eq)) throw exception("equilibria use multiple <%s>", (eq->name)());

            // update status
            coerce(width) = max_of(width,pEq->name.size());
            update(*pEq,size());
            return *pEq;
        }

        const enode * equilibria:: head() const throw()
        {
            return (*db.tree).head;
        }


        const equilibrium & equilibria:: operator[](const size_t indx) const throw()
        {
            assert(indx>=1);
            assert(indx<=size());
            return ***(*db.tree).get(indx);
        }

        


        size_t equilibria:: size() const throw()
        {
            return db.size();
        }

        std::ostream & operator<<(std::ostream &os, const equilibria &eqs)
        {
            os << '{' << std::endl;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eq.display(os << "  (*) ", eqs.width, 0);
                os << std::endl;
            }
            os << '}';
            return os;
        }

        
        

    }

}


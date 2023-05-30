#include "yack/chem/equilibria.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace Chemical
    {
        const char Equilibria:: CLID[] = "Chemical:: Equilibria";

        Equilibria:: ~Equilibria() noexcept {}

        Equilibria::  Equilibria() noexcept :
        reac(),
        prod(),
        edb()
        {}

        const eList *  Equilibria::  operator->() const noexcept
        {
            return & *edb.get_tree();
        }

        void Equilibria:: add(Equilibrium *eq)
        {
            const Equilibrium::Pointer pEq = eq;
            if(!edb.insert(pEq))
            {
                throw imported::exception(CLID,"multiple equilibrium '%s'", eq->name());
            }
            coerce(eq->indx[TopLevel]) = edb.size();
            update(*eq);
        }

        void Equilibria:: finalize() noexcept
        {
            coerce(reac.maxlen) = 0;
            coerce(prod.maxlen) = 0;

            for(const eNode *node=(*this)->head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                reac.update(eq.reac);
                prod.update(eq.prod);
            }
        }

        std::ostream & operator<<(std::ostream &os, const Equilibria &eqs)
        {
            os << '{' << std::endl;
            for(const eNode *node=eqs->head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                eqs.pad(os << ' ' << eq.name,eq) << " : ";
                eqs.reac.pad(os << eq.reac.name,eq.reac);
                os << " <=> ";
                eqs.prod.pad(os << eq.prod.name,eq.prod);
                os << " : ";
                os << std::endl;
            }
            os << '}';
            return os;
        }
    }

}


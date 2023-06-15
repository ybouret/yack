#include "yack/chem/equilibria.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace Chemical
    {
        const char Equilibria:: CLID[] = "Chemical:: Equilibria";

        Equilibria:: ~Equilibria() noexcept {}

        Equilibria::  Equilibria() noexcept :
        Gathering(),
        reac(),
        prod(),
        edb()
        {}

        Equilibria:: Equilibria(const Equilibria &other) :
        Gathering(other),
        reac(other.reac),
        prod(other.prod),
        edb(other.edb)
        {
        }
        

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


        std::ostream & Equilibria:: display(std::ostream &os, const Equilibrium &eq) const
        {
            pad(os << ' ' << eq.name,eq) << " : ";
            reac.pad(os << eq.reac.name,eq.reac);
            os << " <=> ";
            prod.pad(os << eq.prod.name,eq.prod);
            os << " : ";
            return os;
        }

        std::ostream & operator<<(std::ostream &os, const Equilibria &eqs)
        {
            os << '{' << std::endl;
            for(const eNode *node=eqs->head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                eqs.display(os,eq) << std::endl;
            }
            os << '}';
            return os;
        }

        void Equilibria:: computeScaling(Extended::Vector       &S,
                                         const Extended::Vector &K) const
        {
            assert(S.size()==K.size());
            for(const eNode *node=(*this)->head;node;node=node->next)
            {
                const Equilibrium   &eq = ***node;
                const size_t         ei = eq.indx[TopLevel];

                if(eq->size && (0==eq.reac.size||0==eq.prod.size) )
                {
                    const Extended::Real k  = K[ei]; assert(k>0);
                    S[ei] = k.pow(eq.sexp);
                }
                else
                {
                    S[ei] = 1;
                }
            }
        }

    }

}


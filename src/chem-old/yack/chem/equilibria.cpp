
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

        void equilibria:: add( const equilibrium::pointer &pEq )
        {
            const equilibrium &eq = *pEq;
            const size_t       ei = *eq;
            const char        *id = eq.name();

            if(1+size()!=ei)    throw exception("add invalid equilibrium '%s' index=%u",id,unsigned(ei));
            if(!db.insert(pEq)) throw exception("add multiple equilibrium '%s'",id);
            assert(ei==size());
            update_with(eq);
        }


        equilibria & equilibria:: operator<<(const equilibria &other)
        {
            assert(this != &other);
            for(const enode *eq = other.head();eq;eq=eq->next)
                add( **eq );
            return *this;
        }

        void equilibria:: build(matrix<bool> &detached) const
        {
            const size_t n = size();
            detached.make(n,n);
            for(const enode *x=head();x;x=x->next)
            {
                const equilibrium &ex = ***x;
                const size_t       ix = *ex;
                writable<bool>    &dx = detached[ix];
                dx[ix] = false;
                for(const enode *y=x->next;y;y=y->next)
                {
                    const equilibrium &ey = ***y;
                    const size_t       iy = *ey;
                    detached[iy][ix] = dx[iy] = ex.detached(ey);
                }
            }
        }

    }

}


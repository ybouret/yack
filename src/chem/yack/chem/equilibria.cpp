
#include "yack/aqueous/equilibria.hpp"
#include "yack/system/exception.hpp"
#include "yack/data/dinky/core-list.hpp"

namespace yack
{
    namespace aqueous
    {
        equilibria:: equilibria() noexcept : gathering(), edb()
        {

        }

        equilibria:: ~equilibria() noexcept
        {

        }

        const eqs_db::tree_type & equilibria:: operator->() const noexcept
        {
            return edb.tree;
        }


        equilibria:: equilibria(const equilibria &eqs) : gathering(), edb()
        {
            equilibria &self = *this;
            for(const enode *en=eqs->head;en;en=en->next)
            {
                self( (***en).clone() );
            }
        }


        size_t equilibria:: next_indx() const noexcept
        {
            return edb.tree->size + 1;
        }

        const char * const equilibria:: clid = "equilibria";

        equilibrium & equilibria:: operator()( equilibrium *eq )
        {
            assert(NULL!=eq);
            const equilibrium::ptr p = eq; assert( (*this)->size+1 == p->indx[0]);
            if( !edb.insert(p) ) throw imported::exception(clid,"multiple <%s>",p->name());
            update(*p);
            return *eq;
        }


        std::ostream & operator<<(std::ostream &os, const equilibria &self)
        {
            os << '{' << std::endl;
            core_list<string> data;
            size_t            emax = 0;
            for(const enode *en = self->head; en; en=en->next)
            {
                const equilibrium &eq = ***en;
                const string       cm = eq.to_string();
                data << cm;
                emax = max_of(emax,cm.size());
            }

            const core_list<string>::node_type *dat = data.head;
            for(const enode *en = self->head; en; en=en->next, dat=dat->next)
            {
                const equilibrium &eq = ***en;
                const string      &cm = **dat;
                self.pad(os << '\t' << eq.name,eq) << ": " << cm;
                for(size_t i=cm.size();i<=emax;++i) os << ' ';
                os << " | " << vformat("(%.15g)", coerce(eq).K(eq.t_display));
                os << std::endl;
            }
            os << '}';
            return os;
        }
    }

}


#include "yack/aqueous/equilibria.hpp"
#include "yack/system/exception.hpp"

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

        

        size_t equilibria:: next_indx() const noexcept
        {
            return edb.tree->size + 1;
        }

        const char * const equilibria:: clid = "equilibria";

        equilibrium & equilibria:: operator()( equilibrium *eq )
        {
            assert(NULL!=eq);
            const equilibrium::ptr p = eq;
            if( !edb.insert(p) ) throw imported::exception(clid,"multiple <%s>",p->name());
            update(*p);
            return *eq;
        }




    }

}

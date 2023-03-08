
#include "yack/aqueous/equilibria.hpp"

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

        const eqs_db::tree_type * equilibria:: operator->() const noexcept
        {
            return &edb.tree;
        }

        const eqs_db::tree_type & equilibria:: operator*() const noexcept
        {
            return edb.tree;
        }

        size_t equilibria:: next_indx() const noexcept
        {
            return edb.tree->size + 1;
        }
    }

}

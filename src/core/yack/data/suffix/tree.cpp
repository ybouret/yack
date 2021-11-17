#include "yack/data/suffix/tree.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace kernel
    {
        void suffix_tree_:: duplicate_failure()
        {
            throw exception("unexpected suffix_tree duplicate failure");
        }
    }
}

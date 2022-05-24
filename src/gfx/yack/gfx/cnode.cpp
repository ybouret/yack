
#include "yack/gfx/cnode.hpp"

namespace yack
{
    namespace graphic
    {
        cnode:: ~cnode() throw() {}

        cnode:: cnode(const coord &p) throw() : next(0), prev(0), pos(p) {}

        cnode:: cnode(const cnode &v) throw() : next(0), prev(0), pos(v.pos) {}


        const coord & cnode:: operator*() const throw() { return pos; }

    }
}



#include "yack/memory/small/studio.hpp"

namespace yack
{
    namespace graphic
    {
        YACK_EXCLUSIVE_IMPL(cnode,"gfx.");
    }
}

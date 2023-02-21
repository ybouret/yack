
#include "yack/gfx/cnode.hpp"

namespace yack
{
    namespace graphic
    {
        cnode:: ~cnode() noexcept {}

        cnode:: cnode(const coord &p) noexcept : next(0), prev(0), pos(p) {}

        cnode:: cnode(const cnode &v) noexcept : next(0), prev(0), pos(v.pos) {}


        const coord & cnode:: operator*() const noexcept { return pos; }

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

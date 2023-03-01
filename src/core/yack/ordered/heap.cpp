
#include "yack/ordered/heap.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace low_level
    {
        heap:: ~heap() noexcept
        {
        }

        heap:: heap() noexcept
        {
        }

        const char * heap:: category() const noexcept { return "heap"; }

        void heap:: no_possible_reserve() const
        {
            throw imported::exception(category(), "no possible reverse()");
        }


    }
}

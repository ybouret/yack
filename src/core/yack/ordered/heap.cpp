
#include "yack/ordered/heap.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace low_level
    {

        const char heap::lbrack     = '[';
        const char heap::rbrack     = ']';
        const char heap::separator  = ';';
        
        heap:: ~heap() noexcept
        {
        }

        heap:: heap() noexcept
        {
        }

        const char * heap:: category() const noexcept { return "heap"; }

        void heap:: no_possible_reserve() const
        {
            throw imported::exception(category(), "no possible reserve for compiled-time memory");
        }

        void  heap:: push_on_memory_full() const
        {
            throw imported::exception(category(), "push on full compiled-time memory");
        }
    }
}

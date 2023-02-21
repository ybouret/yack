
#include "yack/container/iterator.hpp"

namespace yack
{
    namespace iterating
    {
        const char *direction_text(const direction d) noexcept
        {
            switch(d)
            {
                case forward: return "iterating::forward";
                case reverse: return "iterating::reverse";
            }
            return yack_unknown;
        }
    }
}


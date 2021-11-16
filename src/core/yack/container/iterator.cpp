
#include "yack/container/iterator.hpp"

namespace yack
{
    namespace iterating
    {
        const char *direction_text(const direction d) throw()
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


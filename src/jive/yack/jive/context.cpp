
#include "yack/jive/context.hpp"

namespace yack
{
    namespace jive
    {
        context:: ~context() throw()
        {
            coerce(line)   = 0;
            coerce(column) = 0;
        }

        context:: context(const context &other) throw() :
        tag(other),
        line(other.line),
        column(other.column)
        {
        }
        


    }
}

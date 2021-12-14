
#include "yack/jive/context.hpp"
#include "yack/exception.hpp"

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
        
        exception &  context:: stamp(exception &excp) const throw()
        {
            return excp.pre("%s:%u:%u: ", (**this)(), line, column);
        }

    }
}

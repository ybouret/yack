
#include "yack/jive/context.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        context:: ~context() noexcept
        {
            coerce(line)   = 0;
            coerce(column) = 0;
        }

        context:: context(const context &other) noexcept :
        tag(other),
        line(other.line),
        column(other.column)
        {
        }
        
        exception &  context:: stamp(exception &excp) const noexcept
        {
            return excp.pre("%s:%u:%u: ", (**this)(), line, column);
        }

        std::ostream & operator<<(std::ostream &os, const context &ctx)
        {
            std::cerr << *ctx << ':' << ctx.line << ':' << ctx.column;
            return os;
        }
        
    }
}

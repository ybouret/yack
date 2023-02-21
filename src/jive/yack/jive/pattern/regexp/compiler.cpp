
#include "yack/jive/pattern/regexp/compiler.hpp"

namespace yack
{
    namespace jive
    {
        const char RXCompiler:: clid[] = "regexp";
        
        RXCompiler:: ~RXCompiler() noexcept
        {

        }

        RXCompiler:: RXCompiler(const char       *entry,
                                const size_t      count,
                                const dictionary *usrDB) noexcept :
        expr(entry),
        curr(expr),
        last(curr+count),
        dict(usrDB),
        deep(0)
        {
            
        }
    }

}

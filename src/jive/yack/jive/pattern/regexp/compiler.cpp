
#include "yack/jive/pattern/regexp/compiler.hpp"

namespace yack
{
    namespace jive
    {
        const char RXCompiler:: clid[] = "regexp";
        
        RXCompiler:: ~RXCompiler() throw()
        {

        }

        RXCompiler:: RXCompiler(const char       *entry,
                                const size_t      count,
                                const dictionary *usrDB) throw() :
        expr(entry),
        curr(expr),
        last(curr+count),
        dict(usrDB),
        deep(0)
        {
            
        }
    }

}

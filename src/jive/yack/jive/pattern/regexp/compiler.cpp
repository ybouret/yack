
#include "yack/jive/pattern/regexp/compiler.hpp"

namespace yack
{
    namespace jive
    {
        const char rx_compiler:: clid[] = "regexp";
        
        rx_compiler:: ~rx_compiler() throw()
        {

        }

        rx_compiler:: rx_compiler(const char       *entry,
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

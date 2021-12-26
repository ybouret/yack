
#include "yack/jive/syntax/rule/wildcard/option.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            option:: ~option() throw()
            {
            }
            
            option:: option(const tag      &name_,
                            const rule     &host_) :
            wildcard(name_,mark,host_)
            {
            }
            
        }
        
    }
}


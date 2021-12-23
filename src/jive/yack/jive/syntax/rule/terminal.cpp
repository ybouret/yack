
#include "yack/jive/syntax/rule/terminal.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            terminal:: ~terminal() throw()
            {
            }

            terminal:: terminal(const tag &t) throw() :
            rule(t,is_terminal,mark)
            {
            }
            
            
        }
    }
}


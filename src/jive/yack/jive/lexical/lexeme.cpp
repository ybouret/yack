
#include "yack/jive/lexical/lexeme.hpp"

namespace yack
{
    namespace jive
    {
        lexeme:: ~lexeme() throw()
        {
        }

        lexeme:: lexeme(const context &ctx) throw() :
        token(),
        context(ctx),
        next(0),
        prev(0)
        {
        }
        
    }

}


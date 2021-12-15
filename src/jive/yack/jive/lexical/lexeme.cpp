
#include "yack/jive/lexical/lexeme.hpp"

namespace yack
{
    namespace jive
    {
        lexeme:: ~lexeme() throw()
        {
        }

        lexeme:: lexeme(const tag     &who,
                        const context &ctx) throw() :
        context(ctx),
        uuid(who),
        data(),
        next(0),
        prev(0)
        {
        }

        std::ostream & operator<<(std::ostream &os, const lexeme &lx)
        {
            const context &ctx = lx;
            os << ctx << ": [" << lx.uuid << "] '" << lx.data << "'";
            return os;
        }

    }

}


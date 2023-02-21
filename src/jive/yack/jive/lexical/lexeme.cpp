
#include "yack/jive/lexical/lexeme.hpp"

namespace yack
{
    namespace jive
    {
        lexeme:: ~lexeme() noexcept
        {
        }

        lexeme:: lexeme(const tag     &who,
                        const context &ctx,
                        const size_t   idx) noexcept :
        context(ctx),
        name(who),
        indx(idx),
        next(0),
        prev(0),
        data()
        {
            assert(idx>0);
        }

        std::ostream & operator<<(std::ostream &os, const lexeme &lx)
        {
            const context &ctx = lx;
            os << ctx << ": [" << lx.name << "] '" << lx.data << "'";
            return os;
        }

        



    }

}



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
        name(who),
        next(0),
        prev(0),
        data()
        {
        }

        std::ostream & operator<<(std::ostream &os, const lexeme &lx)
        {
            const context &ctx = lx;
            os << ctx << ": [" << lx.name << "] '" << lx.data << "'";
            return os;
        }

        token & lexeme::operator*() throw()
        {
            return data;
        }

        const token & lexeme::operator*() const throw()
        {
            return data;
        }



    }

}


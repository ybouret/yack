#include "yack/jive/character.hpp"

namespace yack
{
    namespace jive
    {
        character:: ~character() throw()
        {
        }

        character:: character(const context &ctx, const uint8_t chr) throw() :
        context(ctx),
        next(0),
        prev(0),
        code(chr)
        {

        }

        character:: character(const character &other) throw() :
        context(other),
        next(0),
        prev(0),
        code(other.code)
        {
        }
        

    }
}

#include "yack/memory/small/studio.hpp"

namespace yack
{
    namespace jive
    {
        YACK_EXCLUSIVE_IMPL(character);
    }
}


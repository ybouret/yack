#include "yack/jive/character.hpp"

namespace yack
{
    namespace jive
    {
        character:: ~character() noexcept
        {
        }

        character:: character(const context &ctx, const uint8_t chr) noexcept :
        context(ctx),
        next(0),
        prev(0),
        code(chr)
        {

        }

        character:: character(const character &other) noexcept :
        context(other),
        next(0),
        prev(0),
        code(other.code)
        {
        }
        
        const uint8_t & character:: operator*() const noexcept
        {
            return code;
        }
    }
}

#include "yack/memory/small/studio.hpp"

namespace yack
{
    namespace jive
    {
        YACK_EXCLUSIVE_IMPL(character,"jive.");
    }
}


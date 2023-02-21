#include "yack/ios/character.hpp"

namespace yack
{
    namespace ios
    {
        character:: ~character() noexcept
        {
            assert(NULL==next);
            assert(NULL==prev);
        }
        
        character:: character() noexcept :
        next(0), prev(0), code(0) {}
        
        
        character:: character(const uint8_t ch) noexcept :
        next(0), prev(0), code(ch) {}
        
        character:: character(const character &other) noexcept :
        next(0), prev(0), code(other.code) {}
        
        
        character & character:: operator=( const character &other ) noexcept
        {
            code = other.code;
            return *this;
        }
        
        character & character:: operator=( const uint8_t ch ) noexcept
        {
            code = ch;
            return *this;
        }
        
        uint8_t & character:: operator*()       noexcept
        {
            return code;
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
    namespace ios
    {
        YACK_EXCLUSIVE_IMPL(character,"ios.");
    }
}


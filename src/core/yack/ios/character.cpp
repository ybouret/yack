#include "yack/ios/character.hpp"

namespace yack
{
    namespace ios
    {
        character:: ~character() throw()
        {
            assert(NULL==next);
            assert(NULL==prev);
        }
        
        character:: character() throw() :
        next(0), prev(0), code(0) {}
        
        
        character:: character(const uint8_t ch) throw() :
        next(0), prev(0), code(ch) {}
        
        character:: character(const character &other) throw() :
        next(0), prev(0), code(other.code) {}
        
        
        character & character:: operator=( const character &other ) throw()
        {
            code = other.code;
            return *this;
        }
        
        character & character:: operator=( const uint8_t ch ) throw()
        {
            code = ch;
            return *this;
        }
        
        uint8_t & character:: operator*()       throw()
        {
            return code;
        }
        
        const uint8_t & character:: operator*() const throw()
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
        YACK_EXCLUSIVE_IMPL(character);
    }
}


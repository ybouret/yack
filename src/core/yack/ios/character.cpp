
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

#include "yack/type/cstring.h"

namespace yack
{
    namespace ios
    {
        
        characters:: characters() throw() : characters_()
        {
        }

        characters:: characters(const characters &other) :
        characters_(other)
        {
        }

        characters:: ~characters() throw()
        {
        }

        void characters:: load(const void *addr, size_t size)
        {
            assert(!(NULL==addr&&size>0));
            const uint8_t *q    = static_cast<const uint8_t *>(addr);
            while(size-- > 0)
                 add( *(q++) );
        }

        characters & characters:: operator=(const characters &other)
        {
            characters_ &self = *this;
            self = other;
            return *this;
        }


        characters & characters:: operator<<(const uint8_t chr)
        {
            add(chr);
            return *this;
        }

        characters & characters:: operator<<(const char *msg)
        {
            load(msg,yack_cstring_size(msg));
            return *this;
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


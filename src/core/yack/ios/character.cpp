
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

#include <cstring>

namespace yack
{
    namespace ios
    {
        characters:: ~characters() throw() {}
        
        characters:: characters() throw() : characters_()
        {
            
        }
        
        characters:: characters(const characters &other) :
        characters_(other)
        {
        }
        
        characters & characters:: operator= (const characters &other)
        {
            characters_ tmp(other);
            swap_with(tmp);
            return *this;
        }
        
        void characters:: add(const void *addr, size_t size)
        {
            assert( !(NULL==addr&&size>0) );
            const uint8_t *p = static_cast<const uint8_t *>(addr);
            while(size-- > 0)
            {
                (*this) << *(p++);
            }
        }
       
        characters & characters:: operator<<(const uint8_t C)
        {
            push_back( new character(C) );
            return *this;
        }
        
        void characters:: append_front(const uint8_t C)
        {
            push_front( new character(C) );
        }
            
        uint8_t characters:: remove_front()  throw()
        {
            assert(size>0);
            const uint8_t res = **head;
            delete pop_front();
            return res;
        }
        
        uint8_t characters:: remove_back()  throw()
        {
            assert(size>0);
            const uint8_t res = **tail;
            delete pop_back();
            return res;
        }

        
        characters & characters:: operator<<(const char *msg)
        {
            if(msg)
            {
                add(msg,strlen(msg));
            }
            return *this;
        }
        
        characters & characters:: operator<<(const characters &other)
        {
            characters_ tmp(other);
            merge_back(tmp);
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


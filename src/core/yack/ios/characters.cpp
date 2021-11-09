#include "yack/ios/characters.hpp"
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

#include "yack/memory/allocator/legacy.hpp"

namespace yack
{
    namespace ios
    {

        char * characters:: cstr() const
        {
            char *msg = static_cast<char*>(memory::legacy::acquire(size+1)); assert(0==msg[size]);
            {
                char *tgt = msg;
                for(const character *ch=head;ch;ch=ch->next)
                    *(tgt++) = char(**ch);
            }
            return msg;
        }

        void characters:: free(char *msg) throw()
        {
            memory::legacy::release(msg);
        }

    }

}

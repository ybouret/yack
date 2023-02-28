#include "yack/ios/characters.hpp"
#include "yack/type/cstring.h"

namespace yack
{
    namespace ios
    {

        characters:: characters() noexcept : characters_()
        {
        }

        characters:: characters(const characters &other) :
        characters_(other) 
        {
        }

        characters:: characters(const char  *msg) :
        characters_()
        {
            if(msg)
            {
            GROW:
                const char c = *(msg++);
                if(0==c) return;
                add(c);
                goto GROW;
            }
        }

        characters:: characters(const uint8_t c) :
        characters_()
        {
            add(c);
        }

        

        characters:: ~characters() noexcept
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

        size_t characters:: fill(char buffer[],const size_t buflen) noexcept
        {
            assert(yack_good(buffer,buflen));

            if(buflen>=size)
            {
                const size_t ans = size;
                while(size>0)
                {
                    *(buffer++) = pull_front();
                }
                return ans;
            }
            else
            {
                assert(buflen<size);
                for(size_t i=buflen;i>0;--i)
                {
                    *(buffer++) = pull_front();
                }
                return buflen;
            }
        }

        characters & characters:: trim(const char *bad, const size_t num) noexcept
        {
            return apply(bad,num, & characters::trim_<is_bad_> );
        }

        characters & characters:: trim(const char *bad) noexcept
        {
            return trim(bad,yack_cstring_size(bad));
        }

        characters & characters:: skip(const char *bad, const size_t num) noexcept
        {
            return apply(bad,num, & characters::skip_<is_bad_> );
        }

        characters & characters:: skip(const char *bad) noexcept
        {
            return skip(bad,yack_cstring_size(bad));
        }

        characters & characters:: strip(const char *bad, const size_t num) noexcept
        {
            return apply(bad,num, & characters::strip_<is_bad_> );
        }

        characters & characters:: strip(const char *bad) noexcept
        {
            return strip(bad,yack_cstring_size(bad));
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




        void characters:: free(char *msg) noexcept
        {
            memory::legacy::release(msg);
        }

    }

}

#include "yack/string.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace ios
    {

        static inline void transfer(string &target, const characters &source)
        {
            for(const character *ch=source.head;ch;ch=ch->next)
                target += char(**ch);
        }

        string characters:: to_string()  const
        {
            string ans(size,as_capacity);
            transfer(ans,*this);
            return ans;
        }

        string *characters:: to_new_string() const
        {
            auto_ptr<string> handle = new string(size,as_capacity);
            transfer(*handle,*this);
            return handle.yield();
        }

    }
}



#include "yack/jive/token.hpp"
#include "yack/ios/ascii/hybrid.hpp"

namespace yack
{
    namespace jive
    {
        token:: ~token() noexcept {}
        token::  token() noexcept       : object(), characters(),  next(0), prev(0) {}
        token::  token(const token &t) : object(), characters(t), next(0), prev(0) {}
        
        std::ostream & operator<< (std::ostream &os, const token &t)
        {
            for(const character *ch=t.head;ch;ch=ch->next)
            {
                os << ios::ascii::hybrid[**ch];
            }
            return os;
        }
        
        const context & token:: operator*() const noexcept
        {
            assert(size>0);
            return *head;
        }

        string token:: to_string(size_t nskip, size_t ntrim) const
        {
            const size_t removed = nskip+ntrim;
            if(removed>=size)
            {
                return string();
            }
            else
            {
                size_t           len = size-removed;
                string           res(len,as_capacity);
                const character *chr = head;
                while(nskip-- > 0) chr=chr->next;
                while(len--   > 0)
                {
                    res += char(**chr);
                    chr=chr->next;
                }
                return res;
            }

        }

        
    }

}

#include "yack/apex/natural.hpp"
#include "yack/exception.hpp"
#include "yack/ios/ascii/hybrid.hpp"

namespace yack
{
    namespace jive
    {
        apn token:: to_apn(const char *which) const
        {
            static const char fn[] = "jive::token::to_apn: ";
            if(!which) which = yack_unknown;

            if(size<=0) throw exception("%s '%s' is empty",fn,which);

            apn res;
            for(const character *ch=head;ch;ch=ch->next)
            {
                res *= 10;
                switch(ch->code)
                {
                    case '0': break;
                    case '1': ++res; break;
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        res += (ch->code-'0');
                        break;

                    default:
                        exception("%s '%s' has invalid '%s'",fn,which, ios::ascii::hybrid[ch->code]);
                }
            }
            return res;
        }
    }
}

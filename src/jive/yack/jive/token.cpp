
#include "yack/jive/token.hpp"
#include "yack/ios/ascii/hybrid.hpp"

namespace yack
{
    namespace jive
    {
        token:: ~token() throw() {}
        token::  token() throw()       : characters(),  next(0), prev(0) {}
        token::  token(const token &t) : characters(t), next(0), prev(0) {}
        
        std::ostream & operator<< (std::ostream &os, const token &t)
        {
            for(const character *ch=t.head;ch;ch=ch->next)
            {
                os << ios::ascii::hybrid[**ch];
            }
            return os;
        }
        
        const context & token:: operator*() const throw()
        {
            assert(size>0);
            return *head;
        }

        
    }

}


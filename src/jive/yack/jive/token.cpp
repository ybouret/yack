
#include "yack/jive/token.hpp"

namespace yack
{
    namespace jive
    {
        token:: ~token() throw() {}
        token::  token() throw()       : characters(),  next(0), prev(0) {}
        token::  token(const token &t) : characters(t), next(0), prev(0) {}
        

    }

}


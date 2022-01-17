#include "yack/string/tokenizer.hpp"
#include "yack/type/cstring.h"

namespace yack
{
    tokenizer:: ~tokenizer() throw()
    {
    }

    tokenizer:: tokenizer(const string &_) throw() :
    tokenizer_(_)
    {
    }

    tokenizer:: tokenizer(const char *text) throw() :
    tokenizer_(text,yack_cstring_size(text))
    {
    }
    


}


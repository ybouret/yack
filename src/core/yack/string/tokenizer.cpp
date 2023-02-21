#include "yack/string/tokenizer.hpp"
#include "yack/type/cstring.h"

namespace yack
{
    tokenizer:: ~tokenizer() noexcept
    {
    }

    tokenizer:: tokenizer(const string &_) noexcept :
    tokenizer_(_)
    {
    }

    tokenizer:: tokenizer(const char *text) noexcept :
    tokenizer_(text,yack_cstring_size(text))
    {
    }
    


}


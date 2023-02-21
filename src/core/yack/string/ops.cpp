#include "yack/string/ops.hpp"

#include <cctype>

namespace yack
{

    static inline void _lo(char &C) noexcept
    {
        C = char( tolower(C) );
    }

    void  strops:: make_lower(string &s) noexcept
    {
        for(size_t i=s.size();i>0;--i)
        {
            _lo(s[i]);
        }
    }

    static inline void _up(char &C) noexcept
    {
        C = char( toupper(C) );
    }

    void  strops:: make_upper(string &s) noexcept
    {
        for(size_t i=s.size();i>0;--i)
        {
            _up(s[i]);
        }
    }
}




#include "yack/check/expression.hpp"
#include <cstring>
#include <iostream>

namespace yack
{

    size_t check_expression::width = 20;

    void   check_expression::xraise(const char *expr)
    {
        assert(NULL!=expr);
        exception excp;
        excp << yack_failure << ':' << '[' << expr << ']';
        throw excp;
    }

    void check_expression:: prolog(const char *expr)
    {
        assert(NULL!=expr);
        const size_t size = strlen(expr);
        std::cerr << '[' << expr << ']';
        for(size_t i=size;i<width;++i) std::cerr << ' ';
    }

    void check_expression::epilog(const bool flag)
    {
        std::cerr << '[' << (flag?yack_success:yack_failure) << ']' << std::endl;
    }
}




//! \file

#ifndef YACK_CHECK_EXPR_INCLUDED
#define YACK_CHECK_EXPR_INCLUDED 1

#include "yack/exception.hpp"

namespace yack
{

    //! helper for testing conditions
    struct check_expression
    {
        static size_t width;                     //!< to align EXPR
        static void   prolog(const char *expr);  //!< prolog, aligned
        static void   epilog(const bool  flag);  //!< epilog success/failure
        static void   xraise(const char *expr);  //!< raise exception
    };

    //! silent assertion, throw on error
#define YACK_ASSERT(EXPR) do { \
/**/ if(!(EXPR)) yack::check_expression::xraise(#EXPR);\
/**/ } while(false)

    //! verbose assertion, throw on error
#define YACK_CHECK(EXPR) do {                        \
/**/ const char *expr = #EXPR;                       \
/**/ yack::check_expression::prolog(expr);           \
/**/ const bool  flag = (EXPR);                      \
/**/ yack::check_expression::epilog(flag);           \
/**/ if(!flag) yack::check_expression::xraise(expr); \
/**/ } while(false)


}

#endif

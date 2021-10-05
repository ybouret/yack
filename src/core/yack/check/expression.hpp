

//! \file

#ifndef YACK_CHECK_EXPR_INCLUDED
#define YACK_CHECK_EXPR_INCLUDED 1

#include "yack/exception.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //! helper for testing conditions
    //__________________________________________________________________________
    struct check_expression
    {
        static size_t width;                     //!< to align EXPR
        static void   prolog(const char *expr);  //!< prolog, aligned
        static void   epilog(const bool  flag);  //!< epilog success/failure
        static void   xraise(const char *expr);  //!< raise exception
    };

    //__________________________________________________________________________
    //
    //! silent assertion, throw on error
    //__________________________________________________________________________
#define YACK_ASSERT(EXPR) do { \
/**/ if(!(EXPR)) yack::check_expression::xraise(#EXPR);\
/**/ } while(false)

    //__________________________________________________________________________
    //
    //! verbose assertion, throw on error
    //__________________________________________________________________________
#define YACK_CHECK(EXPR) do {                        \
/**/ const char *expr = #EXPR;                       \
/**/ yack::check_expression::prolog(expr);           \
/**/ const bool  flag = (EXPR);                      \
/**/ yack::check_expression::epilog(flag);           \
/**/ if(!flag) yack::check_expression::xraise(expr); \
/**/ } while(false)


}

#endif

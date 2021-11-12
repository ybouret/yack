
//! \file
#ifndef YACK_HASHING_TESTING_INCLUDED
#define YACK_HASHING_TESTING_INCLUDED 1


#include "yack/setup.hpp"

namespace yack
{

    namespace hashing
    {
        class function;
        
        struct testing
        {
            const char *text; //!< to process
            const char *hash; //!< hexadecimal result
            static void run( function &H, const testing tests[], const size_t count );
        };
#define YACK_HASHING_TEST(FUNCTION,TESTS) do { \
/**/    FUNCTION H;\
/**/    testing::run(H,TESTS,sizeof(TESTS)/sizeof(TESTS[0]));\
/**/  } while(false)

    }

}

#endif

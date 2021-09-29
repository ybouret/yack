//! \file

#ifndef YACK_PRINTF_CHECK_INCLUDED
#define YACK_PRINTF_CHECK_INCLUDED 1

//! \warning for C++ methods, increase count by 1!
#if defined(__GNUC__)
#    define YACK_PRINTF_CHECK(i,j) __attribute__ ((format (printf, i, j)))
#else
#    define YACK_PRINTF_CHECK(i,j)
#endif

//! default C++ formating methods
#define YACK_PRINTF_API YACK_PRINTF_CHECK(2,3)

#endif

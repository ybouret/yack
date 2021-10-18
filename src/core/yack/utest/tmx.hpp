
//! \file

#ifndef YACK_UTEST_TMX_INCLUDED
#define YACK_UTEST_TMX_INCLUDED 1

#include "yack/system/wtime.hpp"
#include <iostream>

//! append ticks to run code
#define YACK_TMX_ADD(TICKS,CODE) do {\
/**/    const uint64_t __yack_tmx_mark = wtime::ticks();\
/**/    do { CODE; } while(false);\
/**/    (TICKS) += wtime::ticks() - __yack_tmx_mark;\
/**/  } while(false)

//! compute rate of code during duration
#define YACK_TMX_RATE(RATE,CODE,DURATION) do {  \
/**/    uint64_t    __yack_ticks = 0;           \
/**/    uint64_t    __yack_count = 0;           \
/**/    double      __yack_rtime = 0;           \
/**/    yack::wtime __yack_wtime;               \
/**/    do { YACK_TMX_ADD(__yack_ticks,CODE ); ++__yack_count; }      \
/**/    while( (__yack_rtime=__yack_wtime(__yack_ticks)) < DURATION );\
/**/    (RATE) = __yack_count/__yack_rtime;                           \
/**/  } while(false)


#endif

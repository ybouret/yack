//! \file

#ifndef YACK_UTEST_RUN_INCLUDED
#define YACK_UTEST_RUN_INCLUDED 1

#include "yack/system/setup.h"
#include <iostream>

//! start a new test
#define YACK_UTEST(NAME) \
/**/  int yack_test_##NAME(int argc, const char **argv) throw() {\
/**/  const char *program = argv[0]; (void) argc; (void) program;


//! leave the test
#define YACK_UDONE() \
/**/    return 0;    \
/**/  }
#endif

//! \file

#ifndef YACK_UTEST_RUN_INCLUDED
#define YACK_UTEST_RUN_INCLUDED 1

#include "yack/setup.h"

#define YACK_UTEST(NAME) \
/**/ extern "C" int YACK_Test_##NAME(int argc, const char **argv) throw() {\
/**/ const char *program = argv[0]; (void) argc; (void) program;


#define YACK_LEAVE() \
/**/    return 0;    \
/**/  }
#endif

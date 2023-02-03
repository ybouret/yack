#include "yack/utest/run.hpp"
#include "matte.h"

using namespace yack;

YACK_UTEST(matte)
{
    MakeWindowMatte()->show(argc,argv);
    
    (void)Fl::run();
}
YACK_UDONE()

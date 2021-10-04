

#include "yack/synchronic/fake-lock.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;



YACK_UTEST(sync_lockable)
{
    fake_lock access;

    
    YACK_LOCK(access);
    YACK_LOCK(access);
    if( access.try_lock() )
    {
        access.unlock();
    }
}
YACK_UDONE()


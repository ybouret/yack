
#include "yack/latch.hpp"
#include "yack/system/error.hpp"
#include <cerrno>

namespace yack
{

    latch:: latch() throw() : on(false)
    {
    }
    
    latch:: ~latch() throw()
    {
    }

    void latch:: lock() throw()
    {
        if(on) system_error::critical_bsd(EINVAL,"already latched");
        on = true;
    }

    void latch:: unlock() throw()
    {
        if(!on) system_error::critical_bsd(EINVAL,"not latched");
        on = false;
    }

    bool latch:: try_lock() throw()
    {
        if(on)
        {
            return false;
        }
        else
        {
            on = true;
            return true;
        }
    }




}



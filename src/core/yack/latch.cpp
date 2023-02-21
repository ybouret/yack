
#include "yack/latch.hpp"
#include "yack/system/error.hpp"
#include <cerrno>

namespace yack
{

    latch:: latch() noexcept : on(false)
    {
    }
    
    latch:: ~latch() noexcept
    {
    }

    void latch:: lock() noexcept
    {
        if(on) system_error::critical_bsd(EINVAL,"already latched");
        on = true;
    }

    void latch:: unlock() noexcept
    {
        if(!on) system_error::critical_bsd(EINVAL,"not latched");
        on = false;
    }

    bool latch:: try_lock() noexcept
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

    bool latch:: latched() const noexcept
    {
        return on;
    }




}



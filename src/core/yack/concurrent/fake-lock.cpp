

#include "yack/concurrent/fake-lock.hpp"
#include "yack/system/error.hpp"
#include <cerrno>

namespace yack
{

    fake_lock::  fake_lock() noexcept : lockable(), depth(0) {}
    fake_lock:: ~fake_lock() noexcept
    {
        if(0!=depth)
        {
            system_error::critical_bsd(EINVAL,"fake lock is not clean");
        }
    }

    void fake_lock:: lock() noexcept { ++coerce(depth);  }

    void fake_lock:: unlock() noexcept
    {
        if( --coerce(depth) < 0 ) system_error::critical_bsd(EINVAL,"fake unlocking failure");
    }

    bool fake_lock:: try_lock() noexcept
    {
        lock(); return true;
    }

}

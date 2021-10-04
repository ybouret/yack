

#include "yack/synchronic/fake-lock.hpp"
#include "yack/system/error.hpp"
#include <cerrno>

namespace yack
{

    fake_lock::  fake_lock() throw() : lockable(), depth(0) {}
    fake_lock:: ~fake_lock() throw()
    {
        if(0!=depth)
        {
            system_error::critical_bsd(EINVAL,"fake lock is not clean");
        }
    }

    void fake_lock:: lock() throw() { ++coerce(depth);  }

    void fake_lock:: unlock() throw()
    {
        if( --coerce(depth) < 0 ) system_error::critical_bsd(EINVAL,"fake unlocking failure");
    }

    bool fake_lock:: try_lock() throw()
    {
        lock(); return true;
    }

}

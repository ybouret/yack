
#include "yack/lockable.hpp"

namespace yack
{

    lockable::  lockable() noexcept {}
    lockable:: ~lockable() noexcept {}

    lockable:: scope:: scope(lockable &args) noexcept : host(args)
    {
        host.lock();
    }

    lockable:: scope:: ~scope() noexcept
    {
        host.unlock();
    }

    return_unlocked::  return_unlocked(lockable &args) noexcept : host(args) {}
    return_unlocked:: ~return_unlocked() noexcept { host.unlock(); }

}

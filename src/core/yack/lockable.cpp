
#include "yack/lockable.hpp"

namespace yack
{

    lockable::  lockable() throw() {}
    lockable:: ~lockable() throw() {}

    lockable:: scope:: scope(lockable &args) throw() : host(args)
    {
        host.lock();
    }

    lockable:: scope:: ~scope() throw()
    {
        host.unlock();
    }

    return_unlocked::  return_unlocked(lockable &args) throw() : host(args) {}
    return_unlocked:: ~return_unlocked() throw() { host.unlock(); }

}


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

}

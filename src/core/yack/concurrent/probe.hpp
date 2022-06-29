//! \file

#ifndef YACK_CONCURRENT_PROBE_INCLUDED
#define YACK_CONCURRENT_PROBE_INCLUDED 1

#include "yack/lockable.hpp"

//! emergency probing of an expresssion
#define YACK_LOCK_PROBE(HOST,EXPRESSION) do {     \
/**/    yack::lockable & the_lock = (HOST);       \
/**/    while(true) {                             \
/**/        if(the_lock.try_lock()) {             \
/**/            const yack::return_unlocked       \
/**/            will_restore(the_lock);           \
/**/            if(EXPRESSION) break;             \
/**/        }                                     \
/**/    }                                         \
} while(false)

#endif


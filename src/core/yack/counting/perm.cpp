#include "yack/counting/perm.hpp"
#include "yack/counting/c/perm.h"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    permutation:: ~permutation() throw()
    {
    }
    
    static inline
    size_t check_perm(const size_t n)
    {
        if(n<=0) throw libc::exception(EDOM,"permutation(0)");
        return n;
    }
    
    permutation:: permutation(const size_t n) :
    schedule(sizeof(yack_perm),check_perm(n))
    {
        yack_perm_init( static_cast<yack_perm*>(addr), n);
        yack_perm_boot( static_cast<yack_perm*>(addr),data);
    }
    
    size_t permutation:: size() const throw()
    {
        return static_cast<yack_perm*>(addr)->n;
    }

    void permutation:: boot() throw()
    {
        yack_perm_boot( static_cast<yack_perm*>(addr),data);
    }
    
    bool  permutation::  next() throw()
    {
        return  yack_perm_next( static_cast<yack_perm*>(addr),data) != 0;
    }
    
}

#include "yack/counting/perm.hpp"
#include "yack/counting/c/perm.h"
#include "yack/apex/natural.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    permutation:: ~permutation() noexcept
    {
    }
    
    static inline
    size_t check_perm(const size_t n)
    {
        if(n<=0) throw libc::exception(EDOM,"permutation(0)");
        return n;
    }

    static inline
    cardinality_t perm_card(const size_t n)
    {
        const apn     num = apn::factorial(n);
        cardinality_t res = 0;
        if(!num.try_cast_to(res))
            throw libc::exception(ERANGE,"permutation(%lu)",(unsigned long)n);
        return res;
    }

    permutation:: permutation(const size_t n) :
    schedule(sizeof(yack_perm),n,perm_card(check_perm(n)))
    {
        yack_perm_init( static_cast<yack_perm*>(addr), n);
        yack_perm_boot( static_cast<yack_perm*>(addr),data);
    }
    
    size_t permutation:: size() const noexcept
    {
        return static_cast<yack_perm*>(addr)->n;
    }

    void permutation:: on_boot() noexcept
    {
        yack_perm_boot( static_cast<yack_perm*>(addr),data);
    }
    
    bool  permutation::  on_next() noexcept
    {
        return  yack_perm_next( static_cast<yack_perm*>(addr),data) != 0;
    }
    
}

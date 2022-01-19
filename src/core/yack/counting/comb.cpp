
#include "yack/counting/comb.hpp"
#include "yack/counting/c/comb.h"
#include "yack/apex/natural.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>


namespace yack
{
    combination:: ~combination() throw()
    {
    }
    
    static size_t check_comb(const size_t n, const size_t k)
    {
        if(n<=0||k<=0||k>n) throw libc::exception(EDOM,"combination(%u,%u)", unsigned(n), unsigned(k) );
        return k;
    }

    static inline
    cardinality_t comb_card(const size_t n, const size_t k)
    {

        const apn     num = apn::comb(n,k);
        cardinality_t res = 0;
        if(!num.try_cast_to(res))
        {
            throw libc::exception(ERANGE,"too many combination(%lu,%lu)", (unsigned long)n, (unsigned long)k );
        }
        return res;
    }

    combination:: combination(const size_t n, const size_t k) :
    schedule(sizeof(yack_comb),k,comb_card(n,check_comb(n,k)))
    {
        yack_comb_init(static_cast<yack_comb*>(addr),n,k);
        yack_comb_boot(static_cast<yack_comb*>(addr),data);
    }
    
    void combination:: boot() throw()
    {
        yack_comb_boot( static_cast<yack_comb*>(addr),data);
    }
    
    bool  combination::  next() throw()
    {
        return  yack_comb_next( static_cast<yack_comb*>(addr),data) != 0;
    }
    
    size_t combination:: size() const throw()
    {
        return static_cast<const yack_comb*>(addr)->k;
    }
    
}

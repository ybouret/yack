
#include "yack/counting/part.hpp"
#include "yack/counting/c/part.h"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    partition:: ~partition() throw()
    {
    }
    
    static inline
    size_t check_part(const size_t n)
    {
        if(n<=0) throw libc::exception(EDOM,"partition(0)");
        return n;
    }
    
    partition:: partition(const size_t n) :
    schedule(sizeof(yack_part),check_part(n))
    {
        yack_part_init( static_cast<yack_part*>(addr), n);
        yack_part_boot( static_cast<yack_part*>(addr),data);
    }
    
    size_t partition:: size() const throw()
    {
        return static_cast<yack_part*>(addr)->k;
    }
    
    void partition:: boot() throw()
    {
        yack_part_boot( static_cast<yack_part*>(addr),data);
    }
    
    bool  partition::  next() throw()
    {
        return  yack_part_next( static_cast<yack_part*>(addr),data) != 0;
    }
    
}

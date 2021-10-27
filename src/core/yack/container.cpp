
#include "yack/container.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    container:: ~container() throw()
    {
    }
    
    container:: container() throw()
    {
    }
    
    size_t container:: next_capacity(const size_t capa)
    {
        static const size_t mini = YACK_MEMALIGN(1);
        const size_t        half = max_of(capa>>1,mini);
        size_t              next = capa+half;
        next = YACK_MEMALIGN(next);
        if(next<=capa) throw libc::exception(EINVAL,"container capacity overflow");
        return next;
    }

    size_t  container::  next_increase(const size_t capa)
    {
        return  next_capacity(capa)-capa;
    }


}

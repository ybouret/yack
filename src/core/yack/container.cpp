
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

    void container:: ensure(const size_t minimal_capacity)
    {
        const size_t curent_capacity = capacity();
        if(curent_capacity<minimal_capacity)
        {
            reserve(minimal_capacity-curent_capacity);
            assert(capacity()>=minimal_capacity);
        }
        assert(capacity()>=minimal_capacity);
    }

    void container:: resume(const size_t minimal_capacity)
    {
        free(); ensure(minimal_capacity);
    }


}

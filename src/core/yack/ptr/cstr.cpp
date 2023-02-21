
#include "yack/ptr/cstr.hpp"
#include "yack/memory/allocator/legacy.hpp"
#include "yack/type/cstring.h"
#include <cstring>

namespace yack
{

#define YACK_CSTR_CTOR(N) \
memory::ro_buffer(),      \
length(N),                \
in_mem(length+1),         \
ch( static_cast<char *>(memory::legacy::acquire(in_mem)) )

    cstr_ptr:: cstr_ptr(const size_t n) :
    YACK_CSTR_CTOR(n)
    {

    }


    cstr_ptr:: cstr_ptr(const char *lhs,const char *rhs) :
    YACK_CSTR_CTOR( yack_cstring_size(lhs) + yack_cstring_size(rhs) )
    {
        const size_t l = yack_cstring_size(lhs);
        memcpy(ch,lhs,l);
        memcpy(ch+l,rhs,yack_cstring_size(rhs));
    }

    cstr_ptr:: cstr_ptr(const char *msg) :
    YACK_CSTR_CTOR( yack_cstring_size(msg) )
    {
        memcpy(ch,msg,length);
    }

    cstr_ptr:: cstr_ptr(const cstr_ptr &other) :
    YACK_CSTR_CTOR(other.length)
    {
        memcpy(ch,other.ch,length);
    }


    cstr_ptr:: ~cstr_ptr() noexcept
    {
        memory::legacy::release(ch);
        ch=0;
    }

    const void *cstr_ptr:: ro_addr() const noexcept
    {
        return ch;
    }

    size_t cstr_ptr:: measure() const noexcept
    {
        return length;
    }


    char       & cstr_ptr:: operator[](const size_t indx) noexcept
    {
        assert(indx<length);
        return ch[indx];
    }

    const char & cstr_ptr:: operator[](const size_t indx) const noexcept
    {
        assert(indx<=length);
        return ch[indx];
    }


}


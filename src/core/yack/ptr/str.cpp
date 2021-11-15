
#include "yack/ptr/str.hpp"
#include "yack/memory/allocator/legacy.hpp"
#include <cstring>

namespace yack
{

#define YACK_CSTR_CTOR(N) \
length(N),\
in_mem(length+1),\
ch( static_cast<char *>(memory::legacy::acquire(in_mem)) )

    cstr_ptr:: cstr_ptr(const size_t n) :
    YACK_CSTR_CTOR(n)
    {

    }


    cstr_ptr:: cstr_ptr(const char *msg) :
    YACK_CSTR_CTOR(msg?strlen(msg):0)
    {
        memcpy(ch,msg,length);
    }

    cstr_ptr:: cstr_ptr(const cstr_ptr &other) :
    YACK_CSTR_CTOR(other.length)
    {
        memcpy(ch,other.ch,length);
    }


    cstr_ptr:: ~cstr_ptr() throw()
    {
        memory::legacy::release(ch);
        ch=0;
    }

    const void *cstr_ptr:: ro_addr() const throw()
    {
        return ch;
    }

    size_t cstr_ptr:: measure() const throw()
    {
        return length;
    }


    char       & cstr_ptr:: operator[](const size_t indx) throw()
    {
        assert(indx<length);
        return ch[indx];
    }

    const char & cstr_ptr:: operator[](const size_t indx) const throw()
    {
        assert(indx<=length);
        return ch[indx];
    }


}


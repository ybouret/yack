
//! \file

#ifndef YACK_STACK_INCLUDED
#define YACK_STACK_INCLUDED 1

#include "yack/pipe/pipe.hpp"

namespace yack
{
    

    template <typename T, typename SEQUENCE>
    class stack : public pipe_on<T,pipe::lifo,SEQUENCE>
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef pipe_on<T,pipe::lifo,SEQUENCE> pipe_type;

        inline explicit stack() throw() : pipe_type()  {}
        inline stack(const size_t n)    : pipe_type(n) {}
        inline stack(const stack &_)    : pipe_type(_) {}
        inline virtual ~stack() throw()                {}


    private:
        YACK_DISABLE_ASSIGN(stack);
    };


}


#endif


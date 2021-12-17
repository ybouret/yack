

//! \file

#ifndef YACK_QUEUE_INCLUDED
#define YACK_QUEUE_INCLUDED 1

#include "yack/pipe/pipe.hpp"

namespace yack
{

    

    template <typename T, typename SEQUENCE>
    class queue : public pipe_on<T,pipe::fifo,SEQUENCE>
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef pipe_on<T,pipe::fifo,SEQUENCE> pipe_type;

        inline explicit queue() throw() : pipe_type()  {}
        inline queue(const size_t n)    : pipe_type(n) {}
        inline queue(const queue &_)    : pipe_type(_) {}
        inline virtual ~queue() throw()                {}


    private:
        YACK_DISABLE_ASSIGN(queue);
    };


}


#endif


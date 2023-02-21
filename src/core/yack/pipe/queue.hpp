

//! \file

#ifndef YACK_QUEUE_INCLUDED
#define YACK_QUEUE_INCLUDED 1

#include "yack/pipe/pipe.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! FIFO pipe based on a SEQUENCE
    //
    //__________________________________________________________________________
    template <typename T, typename SEQUENCE>
    class queue : public pipe_on<T,pipe::fifo,SEQUENCE>
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                            //!< aliases
        typedef pipe_on<T,pipe::fifo,SEQUENCE> pipe_type;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit queue() noexcept : pipe_type()  {} //!< setup empty
        inline queue(const size_t n)    : pipe_type(n) {} //!< setup with capacity
        inline queue(const queue &_)    : pipe_type(_) {} //!< copy using sequence
        inline virtual ~queue() noexcept                {} //!< cleanup


    private:
        YACK_DISABLE_ASSIGN(queue);
    };


}


#endif


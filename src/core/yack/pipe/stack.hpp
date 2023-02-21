
//! \file

#ifndef YACK_STACK_INCLUDED
#define YACK_STACK_INCLUDED 1

#include "yack/pipe/pipe.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! LIFO pipe based on a SEQUENCE
    //
    //__________________________________________________________________________
    template <typename T, typename SEQUENCE>
    class stack : public pipe_on<T,pipe::lifo,SEQUENCE>
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                           //!< aliases
        typedef pipe_on<T,pipe::lifo,SEQUENCE> pipe_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit stack() noexcept : pipe_type()  {} //!< setup empty
        inline stack(const size_t n)    : pipe_type(n) {} //!< setup with capacity
        inline stack(const stack &_)    : pipe_type(_) {} //!< copy using sequence
        inline virtual ~stack() noexcept                {} //!< cleanup


    private:
        YACK_DISABLE_ASSIGN(stack);
    };


}


#endif



//! \file

#ifndef YACK_SEQUENCE_INCLUDED
#define YACK_SEQUENCE_INCLUDED 1

#include "yack/container.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    
    template <typename T>
    class sequence : public container
    {
    public:
        YACK_DECL_ARGS(T,type);
        inline virtual ~sequence() throw() {}

        virtual void push_back(param_type args)  = 0;
        virtual void push_front(param_type args) = 0;
        virtual void pop_back()  throw() = 0;
        virtual void pop_front() throw() = 0;
        
    protected:
        inline explicit sequence() throw() : container() {}
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(sequence);
    };
    
}


#endif


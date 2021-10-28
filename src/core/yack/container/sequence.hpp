
//! \file

#ifndef YACK_SEQUENCE_INCLUDED
#define YACK_SEQUENCE_INCLUDED 1

#include "yack/container.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! sequence of objects
    //
    //__________________________________________________________________________
    template <typename T>
    class sequence : public container
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases

        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________
        virtual void push_back(param_type args)  = 0; //!< push back existing object
        virtual void push_front(param_type args) = 0; //!< push front existing object
        virtual void pop_back()  throw() = 0;         //!< pop back object
        virtual void pop_front() throw() = 0;         //!< pop front object

        //______________________________________________________________________
        //
        // non-virtual interface
        //______________________________________________________________________
        inline type       &front()       throw() { return *(type *) _front(); } //!< front item
        inline const_type &front() const throw() { return *         _front(); } //!< front item, const
     
        inline type       &back()       throw() { return *(type *) _back(); }   //!< back item
        inline const_type &back() const throw() { return *         _back(); }   //!< back item, const
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~sequence() throw() {}
    protected:
        inline explicit sequence() throw() : container() {}
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(sequence);
        virtual const_type *_front() const throw() = 0;
        virtual const_type *_back()  const throw() = 0;

    };
    
}


#endif


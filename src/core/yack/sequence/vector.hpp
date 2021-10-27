
//! \file

#ifndef YACK_VECTOR_INCLUDED
#define YACK_VECTOR_INCLUDED 1

#include "yack/container/as-capacity.hpp"
#include "yack/container/sequence.hpp"
#include "yack/sequence/contiguous.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! vector of contiguous objects
    //
    //__________________________________________________________________________
    template <typename T>
    class vector : public sequence<T>, public contiguous<T>
    {
    public:
        YACK_DECL_ARGS(T,type); //!< alias

        inline explicit vector() : count(0), utter(0), base(0), item(0) {}
        inline virtual ~vector() throw() {}

        //______________________________________________________________________
        //
        // collection interface
        //______________________________________________________________________
        inline virtual size_t   size()      const throw() { count; }

        //______________________________________________________________________
        //
        // releasable interface
        //______________________________________________________________________
        inline virtual void     release()         throw() {   }

        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual size_t   capacity()  const throw() { return utter; }
        inline virtual size_t   available() const throw() {  }
        inline virtual void     free()            throw() { }
        inline virtual void     reserve(size_t n) {   }

        //______________________________________________________________________
        //
        // writable interface
        //______________________________________________________________________
        inline type       & operator[](const size_t indx) throw()       { assert(indx>=1); assert(indx<=size()); return item[indx]; }
        inline const_type & operator[](const size_t indx) const throw() { assert(indx>=1); assert(indx<=size()); return item[indx]; }

        //______________________________________________________________________
        //
        //! sequence interface
        //______________________________________________________________________
        inline virtual void push_back(param_type  args) {   }
        inline virtual void push_front(param_type args) {   }
        inline virtual void pop_back()  throw() {    }
        inline virtual void pop_front() throw() {   }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(vector);
        size_t        count; //!< size
        size_t        utter; //!< capacity
        mutable_type *base;  //!< first   object location base[0..count-1]
        mutable_type *item;  //!< shifted object location item[1..count]
    };

}

#endif

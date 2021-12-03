
//! \file

#ifndef YACK_LINKED_PTR_INCLUDED
#define YACK_LINKED_PTR_INCLUDED 1

#include "yack/ptr/arc.hpp"
#include "yack/object.hpp"

namespace yack
{


    //__________________________________________________________________________
    //
    //! counted pointer of counted object
    /**
     * pointee must have void withhold(), bool liberate() and size_t quantity()
     */
    //__________________________________________________________________________
    template <typename T>
    class linked_ptr : public object, public arc_ptr<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef typename ptr<T>::type         type;         //!< alias
        typedef typename ptr<T>::mutable_type mutable_type; //!< alias
        using ptr<T>::pointee;


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup with valid address
        inline linked_ptr(const arc_ptr<T> &user) throw() : arc_ptr<T>(user), next(0), prev(0) { }

        //! create new linked_ptr as node, take care of memory
        static inline linked_ptr *make(type *addr)
        {
            const arc_ptr<T> temp(addr);
            return new linked_ptr(temp);
        }

        //! no-throw copy
        inline linked_ptr(const linked_ptr &_) throw() : arc_ptr<T>(_), next(0), prev(0) {}

        //! no throw assign
        inline linked_ptr & operator=(const linked_ptr &other) throw()
        {
            arc_ptr<T> &lhs = *this; const arc_ptr<T> &rhs = other;
            lhs = rhs;
            return *this;
        }

        //! cleanup
        inline virtual ~linked_ptr() throw() { }

        //______________________________________________________________________
        //
        // extra members
        //______________________________________________________________________
        linked_ptr *next; //!< for list/pool
        linked_ptr *prev; //!< for list
        
    };


}

#endif

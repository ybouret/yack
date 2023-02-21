//! \file


#ifndef YACK_DOUBLY_LINKED_INCLUDED
#define YACK_DOUBLY_LINKED_INCLUDED 1

#include "yack/data/list/cxx.hpp"
//#include "yack/object.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //! helper for constructor
    //__________________________________________________________________________
#define YACK_DOUBLY_LINKED(CODE)   CODE, next(0), prev(0)


    //__________________________________________________________________________
    //
    //
    //! make a doubly linked class
    //
    //__________________________________________________________________________
    template <typename T>
    class doubly_linked :   public T
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef doubly_linked<T>       node_type; //!< alias
        typedef cxx_list_of<node_type> list_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~doubly_linked() noexcept {}

        //! setup with no argument
        inline explicit doubly_linked() :
        YACK_DOUBLY_LINKED(T())
        {
        }

        //! setup with one argument
        template <typename U>
        inline explicit doubly_linked(U u) :
        YACK_DOUBLY_LINKED(T(u))
        {
        }

        //! setup with two arguments
        template <typename U, typename V>
        inline explicit doubly_linked(U u, V v) :
        YACK_DOUBLY_LINKED(T(u,v))
        {
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        doubly_linked *next; //!< for list
        doubly_linked *prev; //!< for list

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(doubly_linked);
    };

}

#endif


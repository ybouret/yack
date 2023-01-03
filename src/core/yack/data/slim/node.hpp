//! \file

#ifndef YACK_DATA_SLIM_NODE_INCLUDED
#define YACK_DATA_SLIM_NODE_INCLUDED 1

#include "yack/object.hpp"
#include "yack/type/args.hpp"
#include "yack/type/destruct.hpp"
#include "yack/type/transmogrify.hpp"
#include <iostream>

namespace yack
{

#define  YACK_SLIM_NODE_CTOR() object(), next(0), prev(0) //!< helper to construct slim_node

    //__________________________________________________________________________
    //
    //
    //! node holding data for list/pool alive or zombie
    //
    //__________________________________________________________________________
    template <typename T>
    class slim_node : public object
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);      //!< aliases
        typedef transmogrify_t alt_t; //!< helper


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        template <typename U>
        inline explicit slim_node(const U &u, const alt_t &) : YACK_SLIM_NODE_CTOR(), data(u)      {} //!< setup from user's args
        inline explicit slim_node() :                    YACK_SLIM_NODE_CTOR(), data()       {} //!< default setup, if exists
        inline explicit slim_node(const slim_node &_) :  YACK_SLIM_NODE_CTOR(), data(_.data) {} //!< copy constructor
        inline virtual ~slim_node() throw() { assert(!next); assert(!prev); }                   //!< cleanup

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline type       & operator*()       throw() { return               data; } //!< access
        inline const_type & operator*() const throw() { return (const_type &)data; } //!< access

        slim_node * zcreate()  { return static_cast<slim_node *>( object::zacquire<slim_node>() ); } //!< create zombie node
        void        zdelete(slim_node *zombie) throw() { assert(zombie); object::zrelease(zombie); } //!< delete zombie node

        //! display wrapper
        inline friend std::ostream & operator<<(std::ostream &os, const slim_node &self) { return os << self.data; }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        slim_node *next; //!< for list/pool
        slim_node *prev; //!< for list
        
    private:
        mutable_type data;
        YACK_DISABLE_ASSIGN(slim_node);
    };

}


#endif


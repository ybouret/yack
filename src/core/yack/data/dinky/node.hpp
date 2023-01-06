//! \file

#ifndef YACK_DATA_DINKY_NODE_INCLUDED
#define YACK_DATA_DINKY_NODE_INCLUDED 1

#include "yack/object.hpp"
#include "yack/type/args.hpp"
#include "yack/type/transmogrify.hpp"
#include <iostream>

namespace yack
{
    
#define  YACK_DINKY_NODE_CTOR() object(), next(0), prev(0) //!< helper to construct 
    
    //__________________________________________________________________________
    //
    //
    //! node holding data for list/pool alive or zombie
    //
    //__________________________________________________________________________
    template <typename T>
    class dinky_node : public object
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
        inline explicit dinky_node(const U &u, const alt_t &) :  YACK_DINKY_NODE_CTOR(), data(u)       {} //!< setup from user's args
        inline explicit dinky_node() :                           YACK_DINKY_NODE_CTOR(), data()        {} //!< default setup, if exists
        inline explicit dinky_node(const dinky_node &_) :        YACK_DINKY_NODE_CTOR(), data(_.data)  {} //!< copy constructor
        inline virtual ~dinky_node() throw() { assert(!next); assert(!prev); }                          //!< cleanup
        
        //______________________________________________________________________
        //
        // access methods
        //______________________________________________________________________
        inline type       & operator*()       throw() { return               data; } //!< access
        inline const_type & operator*() const throw() { return (const_type &)data; } //!< access
        
        //! display wrapper
        inline friend std::ostream & operator<<(std::ostream &os, const dinky_node &self) { return os << self.data; }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        dinky_node *next; //!< for list/pool
        dinky_node *prev; //!< for list
        
    private:
        mutable_type data;
        YACK_DISABLE_ASSIGN(dinky_node);
    };
    
    namespace kernel
    {
        //! helper to sort slim_node<T>
        template <typename NODE, typename FUNC>
        struct primary_sort_callback
        {
            FUNC &func; //!< comparison
            
            //! call
            inline int operator()(const NODE *lhs, const NODE *rhs)
            {
                assert(lhs); assert(rhs);
                return func(**lhs, **rhs);
            }
        };
        
        //! helper to sort slim_node< slim_ptr<T> >
        template <typename NODE, typename FUNC>
        struct replica_sort_callback
        {
            FUNC &func; //!< comparison
            
            //! call
            inline int operator()(const NODE *lhs, const NODE *rhs)
            {
                assert(lhs); assert(rhs);
                return func(***lhs, ***rhs);
            }
        };
        
    }
}


#endif


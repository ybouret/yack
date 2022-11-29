
//! \file

#ifndef YACK_DATA_META_NODE_INCLUDED
#define YACK_DATA_META_NODE_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/object.hpp"
#include "yack/ptr/ptr.hpp"
#include <iostream>

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! meta C++ node
    //
    //__________________________________________________________________________
    template <typename T>
    class meta_node : public object
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type); //!< aliases

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! cleanup
        inline virtual ~meta_node() throw()
        {
            addr = 0;
        }

        //! setup
        inline explicit meta_node(type *user) throw() :
        object(),
        next(0),
        prev(0),
        addr( (mutable_type*)user )
        {}

        //! copy
        inline meta_node(const meta_node &other) throw() :
        object(),
        next(0),
        prev(0),
        addr( (mutable_type*)(other.addr) )
        {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! check
        inline bool is_valid() const throw() { return 0 != addr; }

        //! change linking
        inline void link(type *user) throw() {
            addr = (mutable_type *)user;
        }

        //! access
        inline type       & operator*()      throw() { assert(addr); return *addr; }

        //! access
        inline const_type & operator*() const throw() { assert(addr); return *addr; }

        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const meta_node &self)
        {
            if(self.is_valid())
            {
                os << *self;
            }
            else
            {
                os << ptr_::nil;
            }
            return os;
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        meta_node *next; //!< for list
        meta_node *prev; //!< for list
    private:
        mutable_type *addr;
        YACK_DISABLE_ASSIGN(meta_node);
    };

}

#endif

//! \file

#ifndef YACK_DATA_META_INCLUDED
#define YACK_DATA_META_INCLUDED 1

#include "yack/data/list/cxx.hpp"
#include "yack/type/args.hpp"
#include "yack/object.hpp"

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
        // metohds
        //______________________________________________________________________
        //! check
        inline bool is_valid() const throw() { return 0 != addr; }

        //! access
        inline type       & operator*()      throw() { assert(addr); return *addr; }

        //! access
        inline const_type & operator*() const throw() { assert(addr); return *addr; }

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

    //__________________________________________________________________________
    //
    //
    //! list of meta nodes
    //
    //__________________________________________________________________________
    template <typename T>
    class meta_list : public cxx_list_of< meta_node<T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef meta_node<T>           node_type; //!< alias
        typedef cxx_list_of<node_type> list_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! clenaup
        inline virtual ~meta_list() throw() {}

        //! setup empty
        inline explicit meta_list() throw() : list_type() {}

        //! copy
        inline explicit meta_list(const meta_list &other) : list_type(other) {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! on-the fly push back new meta node with data
        inline meta_list & operator<<( T *user ) { this->push_back( new node_type(user) ); return *this; }


    private:
        YACK_DISABLE_ASSIGN(meta_list);
    };

}

#endif


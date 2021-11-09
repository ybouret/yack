//! \file

#ifndef YACK_DATA_TREE_KNOT_INCLUDED
#define YACK_DATA_TREE_KNOT_INCLUDED 1

#include "yack/object.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/type/args.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! knot to store data beside a tree
    //
    //__________________________________________________________________________
    template <typename T,typename NODE>
    class tree_knot : public object
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                    //!< aliases
        typedef cxx_list_of<tree_knot> list_type;  //!< alias
        typedef cxx_pool_of<tree_knot> pool_type;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit tree_knot() throw() : next(0), prev(0), node(0), data(0), impl() {} //!< setup
        inline virtual ~tree_knot() throw() { if(data) free(); }                            //!< cleanup

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! construct data
        inline void make(const_type &args)
        {
            assert(NULL==data);
            data = new ( out_of_reach::zset(impl,sizeof(impl)) ) mutable_type(args);
        }

        //! destruct data
        inline tree_knot *free() throw()
        {
            assert(NULL!=data);
            data->~mutable_type();
            data = NULL;
            return this;
        }

        inline bool         is_alive()  const throw() { return NULL != data; }              //!< check satus
        inline type       & operator*()       throw() { assert(is_alive()); return *data; } //!< access data
        inline const_type & operator*() const throw() { assert(is_alive()); return *data; } //!< access const data

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        tree_knot    *next; //!< for list/pool
        tree_knot    *prev; //!< for list
        const NODE   *node; //!< node within tree
        
    private:
        mutable_type  *data;
        void          *impl[ YACK_WORDS_FOR(T) ];
        YACK_DISABLE_COPY_AND_ASSIGN(tree_knot);
    };

}

#endif

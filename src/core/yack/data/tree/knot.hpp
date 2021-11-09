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

    template <typename T,typename NODE>
    class tree_knot : public object
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef cxx_list_of<tree_knot> list_type;
        typedef cxx_pool_of<tree_knot> pool_type;

        inline explicit tree_knot() throw() : next(0), prev(0), node(0), data(0), impl() {}
        inline virtual ~tree_knot() throw() { if(data) free(); }

        void make(const_type &args)
        {
            assert(NULL==data);
            data = new ( out_of_reach::zset(impl,sizeof(impl)) ) mutable_type(args);
        }

        inline void free() throw()
        {
            assert(NULL!=data);
            data->~mutable_type();
            data = NULL;
        }

        inline bool is_alive() const throw() { return NULL != data; }
        inline bool is_zombi() const throw() { return NULL == data; }

        inline type       & operator*()       throw() { assert(is_alive()); return *data; }
        inline const_type & operator*() const throw() { assert(is_alive()); return *data; }


        tree_knot    *next;
        tree_knot    *prev;

    private:
        NODE          *node;
        mutable_type  *data;
        void          *impl[ YACK_WORDS_FOR(T) ];
        YACK_DISABLE_COPY_AND_ASSIGN(tree_knot);
    };

}

#endif

//! \file

#ifndef YACK_HASH_SET_INCLUDED
#define YACK_HASH_SET_INCLUDED 1

#include "yack/container/associative.hpp"
#include "yack/data/hash/table.hpp"
#include "yack/associative/hash/key-hasher.hpp"

namespace yack
{


    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY> >
    class hash_set : public associative<KEY,T>
    {
    public:
        YACK_DECL_ARGS(T,type);
        YACK_DECL_ARGS(KEY,key_type);
        class node_type
        {
        public:
            inline ~node_type() throw() {}
            inline  node_type(param_type v) :
            next(0), prev(0), val_(v)
            {
            }

            inline type           & operator*()       throw() { return val_; }
            inline const_type     & operator*() const throw() { return val_; }
            inline const_key_type & key() const throw() { return val_.key(); }

            node_type *next;
            node_type *prev;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(node_type);
            type           val_;
        };

        typedef kernel::hash_table<KEY,node_type> htable_type;

        inline virtual ~hash_set() throw() {}
        inline explicit hash_set() throw() : hash(), table() {}

        const_type *search(param_key_type key) const throw()
        {
            const node_type *node = table.search(hash(key),key);
            return node ? & (**node) : NULL;
        }

        mutable KEY_HASHER  hash;
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_set);
        htable_type table;
    };

}

#endif


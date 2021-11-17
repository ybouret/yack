

//! \file

#ifndef YACK_HASH_MAP_INCLUDED
#define YACK_HASH_MAP_INCLUDED 1

#include "yack/container/associative.hpp"
#include "yack/data/hash/table.hpp"
#include "yack/associative/hash/key-hasher.hpp"

namespace yack
{
    

    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY> >
    class hash_map : public associative<KEY,T>
    {
    public:
        YACK_DECL_ARGS(T,type);
        YACK_DECL_ARGS(KEY,key_type);
        class node_type
        {
        public:
            inline ~node_type() throw() {}
            inline  node_type(param_key_type k, param_type v) :
            next(0), prev(0), key_(k), val_(v)
            {
            }

            inline type       & operator*()       throw() { return val_; }
            inline const_type & operator*() const throw() { return val_; }

            inline const_key_type & key() const throw() { return key_; }

            node_type *next;
            node_type *prev;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(node_type);
            const_key_type key_;
            type           val_;
        };

        typedef kernel::hash_table<KEY,node_type> htable_type;
        
        inline virtual ~hash_map() throw() {}
        inline explicit hash_map() throw() : hash(), table() {}

        const_type *search(param_key_type key) const throw()
        {
            const node_type *node = table.search(hash(key),key);
            return node ? & (**node) : NULL;
        }

        mutable KEY_HASHER  hash;
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_map);
        htable_type table;
    };

}

#endif


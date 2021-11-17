//! \file

#ifndef YACK_HASH_SET_INCLUDED
#define YACK_HASH_SET_INCLUDED 1

#include "yack/associative/hash/compound.hpp"
#include "yack/associative/registry.hpp"

namespace yack
{


    template <typename KEY,typename T>
    class hash_set_node
    {
    public:
        YACK_DECL_ARGS(T,type);
        YACK_DECL_ARGS_(KEY,key_type);

        inline ~hash_set_node() throw() {}
        inline  hash_set_node(param_type v) :
        next(0), prev(0), val_(v)
        {
        }

        inline type           & operator*()       throw() { return val_; }
        inline const_type     & operator*() const throw() { return val_; }
        inline const_key_type & key() const throw() { return val_.key(); }

        hash_set_node *next;
        hash_set_node *prev;
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_set_node);
        type           val_;
    };

    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY> >
    class hash_set : public hash_compound<KEY,T,hash_set_node<KEY,T>,KEY_HASHER,registry>
    {
    public:
        typedef hash_set_node<KEY,T>                               node_type;
        typedef hash_compound<KEY,T,node_type,KEY_HASHER,registry> base_type;
        YACK_DECL_ARGS(T,type);
        YACK_DECL_ARGS(KEY,key_type);
        using base_type::table;
        using base_type::zpool;
        using base_type::hash;
        using base_type::quit;

        inline virtual ~hash_set() throw() {}
        inline explicit hash_set() throw() : base_type()  {}

        //! registry interface
        inline virtual bool insert(param_type args)
        {
            const_key_type &key = args.key();
            if(table.insert(hash(key),key,make(args),zpool,quit))
            {
                this->safe_adjust();
                return true;
            }
            else
            {
                return false;
            }
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_set);

        inline node_type *make(const_type &args)
        {
            return new (zpool.size ? zpool.query() : object::zacquire<node_type>()) node_type(args);
        }
        
    };

}

#endif


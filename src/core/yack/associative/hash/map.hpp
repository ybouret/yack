

//! \file

#ifndef YACK_HASH_MAP_INCLUDED
#define YACK_HASH_MAP_INCLUDED 1

#include "yack/associative/hash/compound.hpp"
#include "yack/associative/glossary.hpp"

namespace yack
{
    template <typename KEY,typename T>
    class hash_map_node
    {
    public:
        YACK_DECL_ARGS(T,type);
        YACK_DECL_ARGS(KEY,key_type);

        inline ~hash_map_node() throw() {}
        inline  hash_map_node(param_key_type k, param_type v) :
        next(0), prev(0), key_(k), val_(v)
        {
        }

        inline type       & operator*()       throw() { return val_; }
        inline const_type & operator*() const throw() { return val_; }

        inline const_key_type & key() const throw() { return key_; }

        hash_map_node *next;
        hash_map_node *prev;
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_map_node);
        const_key_type key_;
        type           val_;
    };

    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY> >
    class hash_map : public hash_compound<KEY,T,hash_map_node<KEY,T>,KEY_HASHER,glossary>
    {
    public:
        YACK_DECL_ARGS(T,type);
        YACK_DECL_ARGS(KEY,key_type);

        typedef hash_map_node<KEY,T>                               node_type;
        typedef hash_compound<KEY,T,node_type,KEY_HASHER,glossary> base_type;
        using base_type::table;
        using base_type::zpool;
        using base_type::quit;
        using base_type::hash;
        
        inline virtual ~hash_map() throw() {}
        inline explicit hash_map() throw() : base_type() {}


        //! glossary interface
        inline virtual bool insert(param_key_type key, param_type args)
        {
            if(table.insert( hash(key),key,make(key,args),zpool,quit))
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
        YACK_DISABLE_COPY_AND_ASSIGN(hash_map);
        inline node_type *make(const_key_type &key, const_type &args)
        {
            return new (zpool.size ? zpool.query() : object::zacquire<node_type>()) node_type(key,args);
        }
    };

}

#endif


//! \file

#ifndef YACK_HASH_SET_INCLUDED
#define YACK_HASH_SET_INCLUDED 1

#include "yack/associative/hash/compound.hpp"
#include "yack/associative/registry.hpp"

namespace yack
{

    namespace kernel
    {
        //______________________________________________________________________
        //
        //! node containing type (with a key() method)
        //______________________________________________________________________
        template <typename KEY,typename T>
        class hash_set_node
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);         //!< aliases
            YACK_DECL_ARGS_(KEY,key_type);  //!< aliases

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            inline hash_set_node(param_type v) : next(0), prev(0), val_(v) {}    //!< setup
            inline                 ~hash_set_node()   throw() {}                 //!< cleanup
            inline type           & operator*()       throw() { return val_; }   //!< access
            inline const_type     & operator*() const throw() { return val_; }   //!< access
            inline const_key_type & key() const throw() { return val_.key(); }   //!< key access

            inline type       * operator->()       throw() { return &val_; } //!< final drilled-down pointer
            inline const_type * operator->() const throw() { return &val_; } //!< final drilled-down pointer


            hash_set_node *next; //!< for list/pool
            hash_set_node *prev; //!< for list
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hash_set_node);
            type           val_;
        };
    }

    //__________________________________________________________________________
    //
    //
    //! registry of type(containing key) bases on a hash table
    //
    //__________________________________________________________________________
    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY> >
    class hash_set : public hash_compound< KEY,T,kernel::hash_set_node<KEY,T>,KEY_HASHER,registry<KEY,T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef kernel::hash_set_node<KEY,T>                                node_type; //!< alias
        typedef hash_compound< KEY,T,node_type,KEY_HASHER,registry<KEY,T> > base_type; //!< alias
        YACK_DECL_ARGS(T,type);                                                        //!< aliases
        YACK_DECL_ARGS(KEY,key_type);                                                  //!< aliases
        using base_type::table;
        using base_type::zpool;
        using base_type::hash;
        using base_type::quit;
        using base_type::release;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! cleanup
        inline virtual ~hash_set() throw() {}
        
        //! setup
        inline explicit hash_set() throw() : base_type()  {}
        
        //! copy
        inline hash_set(const hash_set &other) : base_type()
        {
            try {
                for(const node_type *node= (*other.table).head;node;node=node->next)
                {
                    (void) insert(**node);
                }
            }
            catch(...) { release(); throw; }
        }

        //______________________________________________________________________
        //
        //! registry interface
        //______________________________________________________________________
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
        YACK_DISABLE_ASSIGN(hash_set);

        inline node_type *make(const_type &args)
        {
            return new (zpool.size ? zpool.query() : object::zacquire<node_type>()) node_type(args);
        }
        
    };

}

#endif


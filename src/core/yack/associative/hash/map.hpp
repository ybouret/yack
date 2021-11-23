

//! \file

#ifndef YACK_HASH_MAP_INCLUDED
#define YACK_HASH_MAP_INCLUDED 1

#include "yack/associative/hash/compound.hpp"
#include "yack/associative/glossary.hpp"
#include "yack/ios/fwd.hpp"

namespace yack
{
    namespace kernel
    {
        extern const char hash_map_category[]; //!< "hash_map"

        //______________________________________________________________________
        //
        //
        //! key+data for hash_map
        //
        //______________________________________________________________________
        template <typename KEY,typename T>
        class hash_map_node
        {
        public:
            YACK_DECL_ARGS(T,type);          //!< alias
            YACK_DECL_ARGS(KEY,key_type);    //!< alias

            //! setup
            inline  hash_map_node(param_key_type k, param_type v) :
            next(0), prev(0), key_(k), val_(v) {}

            inline                 ~hash_map_node()   throw() {}               //!< cleanup
            inline type           & operator*()       throw() { return val_; } //!< access
            inline const_type     & operator*() const throw() { return val_; } //!< access
            inline const_key_type & key()       const throw() { return key_; } //!< access


            inline hash_map_node       * operator->()       throw() { return this; } //!< for drill down behavior
            inline const hash_map_node * operator->() const throw() { return this; } //!< for drill down behavior


            hash_map_node *next; //!< for list/pool
            hash_map_node *prev; //!< for list
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hash_map_node);
            const_key_type key_;
            type           val_;
        };
    }


    //__________________________________________________________________________
    //
    //
    //! glossary (key,type) based and a hash table
    //
    //__________________________________________________________________________
    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY> >
    class hash_map : public hash_compound< KEY,T,kernel::hash_map_node<KEY,T>,KEY_HASHER,glossary<KEY,T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                                                        //!< aliases
        YACK_DECL_ARGS(KEY,key_type);                                                  //!< aliases
        typedef kernel::hash_map_node<KEY,T>                                node_type; //!< alias
        typedef hash_compound< KEY,T,node_type,KEY_HASHER,glossary<KEY,T> > base_type; //!< alias
        using base_type::table;
        using base_type::zpool;
        using base_type::quit;
        using base_type::hash;
        using base_type::release;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! cleanup
        inline virtual ~hash_map() throw() {}
        
        //! setup empty
        inline explicit hash_map() throw() : base_type() {}
        
        //! copy
        inline hash_map(const hash_map &other) : collection(), base_type()
        {
            try {
                for(const node_type *node= (*other.table).head;node;node=node->next)
                {
                    (void) insert(node->key(),**node);
                }
            }
            catch(...) { release(); throw; }
        }

        virtual const char *category() const throw() { return kernel::hash_map_category; }


        //______________________________________________________________________
        //
        //! glossary interface
        //______________________________________________________________________
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

        //! output to std::ostream
        friend inline std::ostream & operator<<(std::ostream &os, const hash_map &s)
        {
            return base_type::template display(os, (*s.table).head );
        }

        //! output to ios::ostream
        friend inline ios::ostream & operator<<(ios::ostream &os, const hash_map &s)
        {
            return base_type::template display(os, (*s.table).head );
        }


    private:
        YACK_DISABLE_ASSIGN(hash_map);
        inline node_type *make(const_key_type &key, const_type &args)
        {
            return new (zpool.size ? zpool.query() : object::zacquire<node_type>()) node_type(key,args);
        }

    public:
        //______________________________________________________________________
        //
        // iterators
        //______________________________________________________________________
        typedef iterating::linked<type,node_type,iterating::forward> iterator; //!< forward const iterator
        iterator begin()   throw() { return (*table).head; }                   //!< forward const begin
        iterator end()     throw() { return NULL;          }                   //!< forward const end
    };

}

#endif


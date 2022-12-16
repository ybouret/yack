//! \file

#ifndef YACK_DATA_SET_INCLUDED
#define YACK_DATA_SET_INCLUDED 1

#include "yack/object.hpp"
#include "yack/counted.hpp"
#include "yack/data/list.hpp"
#include "yack/data/pool.hpp"
#include "yack/type/args.hpp"
#include "yack/type/destruct.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{

    template <typename T>
    class ds_node
    {
    public:
        YACK_DECL_ARGS_(T,type);
        inline  ds_node(const_type &args) : next(0), prev(0), data(args) {}
        inline ~ds_node() throw() {}

        ds_node *next;
        ds_node *prev;
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ds_node);
        type     data;
    };

    template <typename T>
    class ds_zpool : public object, public counted
    {
    public:
        typedef ds_node<T> node_type;
        YACK_DECL_ARGS_(T,type);
        inline explicit ds_zpool() throw() : object(), counted() {}
        inline virtual ~ds_zpool() throw() { release(); }

        inline size_t size() const throw() { return repo.size; }

        inline void reserve(size_t n) {
            while(n-- > 0) repo.store( object::zacquire<node_type>() );
        }

        inline void release() throw() {
            while( repo.size ) object::zrelease( repo.query() );
        }

        inline void free(node_type *node) throw() {
            assert(node);
            repo.store( out_of_reach::naught( destructed(node) ) );
        }

        inline node_type *make(const_type &args) {
            node_type *node = repo.size ? repo.query() : object::zacquire<node_type>();
            try {
                return new (node) node_type(args);
            }
            catch(...)  {
                repo.store(node);
                throw;
            }
        }






    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ds_zpool);
        pool_of<node_type> repo;
    };


    template <typename T>
    class data_set
    {
    public:
        YACK_DECL_ARGS(T,type);
        class node_type
        {
        public:
            inline  node_type(const_type &args) : next(0), prev(0), data(args) {}
            inline ~node_type() throw() {}

            node_type *next;
            node_type *prev;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(node_type);
            mutable_type data;
        };

        typedef list_of<node_type> list_type;
        typedef pool_of<node_type> pool_type;

        inline explicit data_set() throw() : alive(), cache() {}
        inline virtual ~data_set() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(data_set);
        list_type alive;
        pool_type cache;

    };

}

#endif

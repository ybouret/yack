

//! \file

#ifndef YACK_LIST_INCLUDED
#define YACK_LIST_INCLUDED 1

#include "yack/container/as-capacity.hpp"
#include "yack/container/sequence.hpp"
#include "yack/container/writable.hpp"
#include "yack/type/destruct.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/args.hpp"
#include "yack/data/list.hpp"
#include "yack/data/pool.hpp"
#include "yack/object.hpp"

namespace yack
{
    
    template <typename T>
    class list : public sequence<T>, public writable<T>
    {
    public:
        YACK_DECL_ARGS(T,type);

        class node_type
        {
        public:
            node_type *next;
            node_type *prev;
            inline  node_type(const_type &args) : next(0), prev(0), data(args) {}
            inline ~node_type() throw() { assert(0==next); assert(0==prev); }
            inline type       &operator*() throw()       { return data; }
            inline const_type &operator*() const throw() { return data; }
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(node_type);
            type data;
        };

        inline explicit         list() throw() : sequence<T>(), writable<T>(), alive(), cache() {}
        inline virtual         ~list() throw() { release(); }
        inline explicit         list(size_t n, const as_capacity_t &) throw() :
        sequence<T>(), writable<T>(), alive(), cache()
        {
            reserve(n);
        }

        // collection interface
        inline virtual size_t   size()      const throw() { return alive.size; }

        //! releasable interfacee
        inline virtual void     release()         throw() { trim_(); kill_(); }

        //! container interface
        inline virtual size_t   capacity()  const throw() { return alive.size+cache.size; }
        inline virtual size_t   available() const throw() { return cache.size; }
        inline virtual void     free()            throw() { }
        inline virtual void     reserve(size_t n) { while(n-- > 0) cache.store( object::zacquire<node_type>()); }

        //! writable interface
        inline type       & operator[](const size_t indx) throw()       { return **alive.get(indx); }
        inline const_type & operator[](const size_t indx) const throw() { return **alive.get(indx); }

        //! sequence interface
        inline virtual void push_back(param_type args)  { alive.push_back( build(args) ); }
        inline virtual void push_front(param_type args) { alive.push_front( build(args)); }
        inline virtual void pop_back()  throw() { free_(alive.pop_back());  }
        inline virtual void pop_front() throw() { free_(alive.pop_front()); }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(list);
        list_of<node_type> alive;
        pool_of<node_type> cache;

        inline void free_( node_type *node )
        {
            assert(node);
            cache.store( static_cast<node_type*>(out_of_reach::zset( destructed(node), sizeof(node_type) ) ) );
        }

        void free_() throw()
        {
            while(alive.size>0) free_(alive.pop_back());
        }

        void kill_() throw() {
            while(alive.size>0) object::zrelease( destructed(alive.pop_back()) );
        }


        void trim_() throw() {
            while(cache.size>0) object::zrelease(cache.query());
        }

        node_type *build(const_type &args)
        {
            node_type *node = cache.size ? cache.query() : object::zacquire<node_type>();
            try
            {
                return new (node) node_type(args);
            }
            catch(...)
            {
                cache.store(node);
                throw;
            }
        }


    };
    
}


#endif


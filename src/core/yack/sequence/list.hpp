

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
#include "yack/data/list/sort.hpp"
#include "yack/data/pool.hpp"
#include "yack/data/nodes-comparison.hpp"
#include "yack/object.hpp"
#include "yack/container/iterator/linked.hpp"

namespace yack
{

    extern const char list_category[]; //!< shared category name
    
    //__________________________________________________________________________
    //
    //
    //! list of objects
    //
    //__________________________________________________________________________
    template <typename T>
    class list : public sequence<T>, public writable<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases

        //! doubly linked node+data
        class node_type
        {
        public:
            inline  node_type(const_type &args) : next(0), prev(0), data(args) {} //!< setup
            inline ~node_type() throw() { assert(0==next); assert(0==prev); }     //!< cleanup
            inline type       &operator*() throw()       { return data; }         //!< access
            inline const_type &operator*() const throw() { return data; }         //!< access, const

            node_type *next; //!< for list
            node_type *prev; //!< for list
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(node_type);
            type data;
        };

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! default setup empty
        inline explicit list() throw() : sequence<T>(), writable<T>(), alive(), cache() {}

        //! cleanup is release
        inline virtual ~list() throw() { release(); }

        //! setup empty, with capacity
        inline explicit list(const size_t n, const as_capacity_t &) throw() :
        sequence<T>(), writable<T>(), alive(), cache()
        {
            reserve(n);
        }

        //! setup with size and default value
        inline explicit list(size_t n, param_type args) :
        sequence<T>(), writable<T>(), alive(), cache()
        {
            try
            {
                while(n-- > 0) alive.push_back( build(args) );
            }
            catch(...) { kill_(); throw; }

        }


        //! hard copy of other list
        inline list(const list &other) :
        sequence<T>(), writable<T>(), alive(), cache()
        {
            try
            {
                for(const node_type *node=other.alive.head;node;node=node->next)
                    alive.push_back( build(**node) );
            }
            catch(...) { kill_(); throw; }
        }


        //______________________________________________________________________
        //
        // collection interface
        //______________________________________________________________________
        inline virtual size_t      size()     const throw() { return alive.size; }
        inline virtual const char *category() const throw() { return list_category; }

        //______________________________________________________________________
        //
        // releasable interface
        //______________________________________________________________________
        inline virtual void     release()         throw() { trim_(); kill_(); }

        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual size_t   capacity()  const throw() { return alive.size+cache.size; }
        inline virtual size_t   available() const throw() { return cache.size; }
        inline virtual void     free()            throw() { }
        inline virtual void     reserve(size_t n) { while(n-- > 0) cache.store( object::zacquire<node_type>()); }

        //______________________________________________________________________
        //
        // writable interface
        //______________________________________________________________________
        inline type       & operator[](const size_t indx) throw()       { return **alive.get(indx); }
        inline const_type & operator[](const size_t indx) const throw() { return **alive.get(indx); }

        //______________________________________________________________________
        //
        //! sequence interface
        //______________________________________________________________________
        inline virtual void push_back(param_type args)  { alive.push_back( build(args) ); }
        inline virtual void push_front(param_type args) { alive.push_front( build(args)); }
        inline virtual void pop_back()  throw() { free_(alive.pop_back());  }
        inline virtual void pop_front() throw() { free_(alive.pop_front()); }


        //______________________________________________________________________
        //
        //! sort
        //______________________________________________________________________
        template <typename COMPARE_DATA> inline
        void sort(COMPARE_DATA &compare_data)
        {
            nodes_comparison<node_type,COMPARE_DATA> compare_nodes = { compare_data };
            merge_list_of<node_type>::sort(alive,compare_nodes);
        }

        //______________________________________________________________________
        //
        //! reverse nodes
        //______________________________________________________________________
        inline void reverse() throw()
        {
            alive.reverse();
        }

        //______________________________________________________________________
        //
        //
        //______________________________________________________________________


    private:
        YACK_DISABLE_ASSIGN(list);
        list_of<node_type> alive;
        pool_of<node_type> cache;

        inline void free_( node_type *node )
        {
            assert(node);
            cache.store( out_of_reach::naught( destructed(node) ) );

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

        inline virtual const_type *_front() const throw()
        { assert(alive.head); return & (**alive.head); }
        
        inline virtual const_type *_back() const throw()
        { assert(alive.tail); return & (**alive.tail); }

    public:
        typedef iterating::linked<type,node_type,iterating::forward> iterator;                           //!< forward iterator
        inline  iterator begin() throw() { return alive.head; }                                          //!< forward iterator begin
        inline  iterator end()   throw() { return 0;          }                                          //!< forward iterator end

        typedef iterating::linked<const_type,const node_type,iterating::forward> const_iterator;         //!< const forward iterator
        inline  const_iterator begin() const throw() { return alive.head; }                              //!< const forward iterator begin
        inline  const_iterator end()   const throw() { return 0;          }                              //!< const forward iterator end

        typedef iterating::linked<type,node_type,iterating::reverse> reverse_iterator;                   //!< reverse iterator
        inline  reverse_iterator rbegin() throw() { return alive.tail; }                                 //!< reverse iterator begin
        inline  reverse_iterator rend()   throw() { return 0;          }                                 //!< reverse iterator end

        typedef iterating::linked<const_type,const node_type,iterating::reverse> const_reverse_iterator; //!< const reverse iterator
        inline  const_reverse_iterator rbegin() const throw() { return alive.tail; }                     //!< const reverse iterator begin
        inline  const_reverse_iterator rend()   const throw() { return 0;          }                     //!< const reverse iterator end
    };
    
}


#endif


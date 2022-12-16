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
#include "yack/ptr/arc.hpp"
#include "yack/signs.hpp"

namespace yack
{

    template <typename T>
    class ds_node
    {
    public:
        YACK_DECL_ARGS_(T,type);
        inline  ds_node(const_type &args) : next(0), prev(0), data(args) {}
        inline ~ds_node() throw() {}

        inline const_type & operator*() const throw() { return data; }
        inline type       & operator*()       throw() { return data; }
        inline friend std::ostream & operator<<(std::ostream &os, const ds_node &self) {
            os << self.data;
            return os;
        }

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
        typedef ds_node<T>        node_type;
        typedef arc_ptr<ds_zpool> pointer;

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
        typedef ds_node<T>         node_type;
        typedef list_of<node_type> list_type;

        typedef typename ds_zpool<type>::pointer zcache;

        inline explicit data_set(const zcache &shared) throw() :
        cache(shared)
        {
        }

        inline virtual ~data_set() throw()
        {
            free();
        }

        inline void free() throw() {
            while(items.size) cache->free(items.pop_back());
        }


        bool insert(param_type args)
        {
            node_type *prev = NULL;
            if(search(args,prev))
                return true;
            else
            {
                node_type *node = cache->make(args);
                if(prev)
                    items.insert_after(prev,node);
                else
                    items.push_front(node);
                return true;
            }


        }

        bool contains(param_type args) const
        {
            node_type *prev = NULL;
            return search(args,prev);
        }

        inline const list_type * operator->() const throw() { return &items; }
        inline const list_type & operator* () const throw() { return  items; }


        
    private:
        list_type items;
        zcache    cache;


        inline bool search(const_type &args, node_type * &prev) const {

            //------------------------------------------------------------------
            //
            // look up in the ordered list
            //
            //------------------------------------------------------------------
            assert(NULL==prev);
            switch(items.size) {
                case  0: return false;                   // not found,
                case  1: return search_first(args,prev); // special case
                default: break;                          // generic case
            }

            assert(items.size>=2);

            //------------------------------------------------------------------
            //
            // compare to head
            //
            //------------------------------------------------------------------
            switch( __sign::of(args,**items.head) ) {
                case negative: return false;                   // smaller than head
                case __zero__: prev = items.head; return true; // found at head
                case positive: break;                          // greater than head
            }


            //------------------------------------------------------------------
            //
            // compare to tail
            //
            //------------------------------------------------------------------
            switch( __sign::of(**items.tail,args) ) {
                case negative: prev = items.tail; return false; // greater than tail
                case __zero__: prev = items.tail; return true;  // found   at tauil
                case positive: break;                           // smaller than tail
            }

            assert(**items.head<args);
            assert(args<**items.tail);

            exit(0);

            return false;
        }

        inline bool search_first(const_type &args, node_type * &prev) const {
            assert(1==items.size);
            assert(NULL==prev);
            switch( __sign::of(args,**items.head) ) {
                case negative: break;
                case __zero__: prev = items.head; return true;
                case positive: prev = items.head; return false;
            }
            return false;
        }


    };

}

#endif

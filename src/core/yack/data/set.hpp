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

    //__________________________________________________________________________
    //
    //
    //! compact data set node
    //
    //__________________________________________________________________________
    template <typename T>
    class ds_node
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type); //!< aliases

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  ds_node(const_type &args) : next(0), prev(0), data(args) {} //!< setup
        inline ~ds_node() throw() {}                                        //!< cleanu[

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline const_type & operator*() const throw() { return data; } //!< access, const

        //! display content
        inline friend std::ostream & operator<<(std::ostream &os, const ds_node &self) {
            os << self.data;
            return os;
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        ds_node *next; //!< for list/pool
        ds_node *prev; //!< for list
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ds_node);
        type     data;
    };

    //__________________________________________________________________________
    //
    //
    //! pool of zombie nodes, to be shared
    //
    //__________________________________________________________________________
    template <typename T>
    class ds_zpool : public object, public counted
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef ds_node<T>        node_type; //!< alias
        typedef arc_ptr<ds_zpool> pointer;   //!< alias
        YACK_DECL_ARGS_(T,type);             //!< aliases

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit ds_zpool() throw() : object(), counted(), repo() {} //!< setup empty
        inline virtual ~ds_zpool() throw() { release(); }                   //!< cleanup

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! reserve n extra nodes
        inline void reserve(size_t n) {
            while(n-- > 0) repo.store( object::zacquire<node_type>() );
        }

        //! release all memory
        inline void release() throw() {
            while( repo.size ) object::zrelease( repo.query() );
        }

        //! free/clean specific node
        inline void free(node_type *node) throw() {
            assert(node); assert(NULL==node->prev); assert(NULL==node->next);
            repo.store( out_of_reach::naught( destructed(node) ) );
        }

        //! create a node from a zombie
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

        //! access for info
        inline const pool_of<node_type> & operator* () const throw() { return repo; }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ds_zpool);
        pool_of<node_type> repo;
    };

    //__________________________________________________________________________
    //
    //
    //! data set of unique/ordered nodes
    //
    //__________________________________________________________________________
    template <typename T>
    class data_set
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                              //!< aliases
        typedef ds_node<T>                       node_type;  //!< alias
        typedef list_of<node_type>               list_type;  //!< alias
        typedef typename ds_zpool<type>::pointer zcache;     //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup with cache
        inline explicit data_set(const zcache &shared) throw() :
        items(),
        cache(shared)
        {
        }

        //! hard copy using cache
        inline data_set(const data_set &other) :
        items(),
        cache(other.cache)
        {
            try {
                for(const node_type *node=other->head;node;node=node->next)
                    items.push_back( cache->make( **node) );
            }
            catch(...) { free(); throw; }
        }

        //! assign by copy/swap
        inline data_set & operator=( const data_set &other )
        {
            data_set tmp(other);
            items.swap_with(tmp.items);
            return *this;
        }

        //! cleanup, send back to cache
        inline virtual ~data_set() throw() { free(); }


        //______________________________________________________________________
        //
        // single items methods
        //______________________________________________________________________

        //! free memory into cache
        inline void free() throw() { while(items.size) cache->free(items.pop_back()); }

        //! try to insert a new value
        bool insert(param_type args)
        {
            node_type *prev = NULL;
            if(search(args,prev))
            {
                return false;
            }
            else
            {
                node_type *node = cache->make(args);
                if(prev)
                {
                    items.insert_after(prev,node);
                }
                else
                {
                    items.push_front(node);
                }
                return true;
            }
        }

        //! test if contains a value
        bool contains(param_type args) const
        {
            node_type *prev = NULL;
            return search(args,prev);
        }

        //! try to remove a value
        bool remove(param_type args)
        {
            node_type *prev = NULL;
            if(search(args,prev))
            {
                assert(NULL!=prev);
                cache->free( items.pop(prev) );
                return true;
            }
            else
            {
                return false;
            }
        }

        //______________________________________________________________________
        //
        // access methods
        //______________________________________________________________________
        inline const list_type * operator->() const throw() { return &items; } //!< access
        inline const list_type & operator* () const throw() { return  items; } //!< access

        //______________________________________________________________________
        //
        // set operations
        //______________________________________________________________________

        
    private:
        list_type items;
        zcache    cache;

        inline bool search(const_type &args, node_type * &prev) const {

            //------------------------------------------------------------------
            //
            // dispatch search
            //
            //------------------------------------------------------------------
            assert(NULL==prev);
            switch(items.size) {
                    //----------------------------------------------------------
                    // empty list
                case  0: return false;
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    // check with head=tail
                case  1: return search_first(args,prev);
                    //----------------------------------------------------------

                default:
                    break;
            }

            //------------------------------------------------------------------
            //
            // look up in the ordered list
            //
            //------------------------------------------------------------------
            return search_local(args,prev);
        }

        inline bool search_first(const_type &args, node_type * &prev) const {
            assert(1==items.size);
            assert(NULL==prev);
            switch( __sign::of(args,**items.head) ) {
                case negative:
                    break;
                case __zero__:
                    prev = items.head;
                    return true;
                case positive:
                    prev = items.head;
                    return false;
            }
            return false;
        }

        inline bool search_local(const_type &args, node_type * &prev) const
        {
            //------------------------------------------------------------------
            //
            // compare to head
            //
            //------------------------------------------------------------------
            node_type *lower = items.head;
            switch( __sign::of(args,**lower) ) {
                case negative:
                    return false;

                case __zero__:
                    prev = lower;
                    return true;

                case positive:
                    break;
            }


            //------------------------------------------------------------------
            //
            // compare to tail
            //
            //------------------------------------------------------------------
            node_type *upper = items.tail;
            switch( __sign::of(args,**upper) )
            {
                case negative:
                    break;

                case __zero__:
                    prev = lower;
                    return true;

                case positive:
                    prev = upper;
                    return false;
            }




        STEP_FORWARD:
            assert(lower!=upper);
            assert(**lower<args);
            assert(args<**upper);
            node_type *forward = lower->next;
            if(forward==upper)
            {
                prev = lower;
                return false;
            }

            switch( __sign::of(args,**forward) )
            {
                case __zero__:
                    prev = forward;
                    return true; // found at forward

                case negative:
                    prev = lower;
                    return false; // between lower and forward

                case positive:
                    lower = forward;
                    break; // move

            }

            goto STEP_FORWARD;
        }


    };

}

#endif

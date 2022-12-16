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
        items(),
        cache(shared)
        {
        }

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

        inline data_set & operator=( const data_set &other )
        {
            data_set tmp(other);
            items.swap_with(tmp.items);
            return *this;
        }


        inline virtual ~data_set() throw() { free(); }

        inline void free() throw() { while(items.size) cache->free(items.pop_back()); }


        bool insert(param_type args)
        {
            std::cerr << "=> inserting " << args << std::endl;
            node_type *prev = NULL;
            if(search(args,prev))
            {
                std::cerr << "--> already" << std::endl;
                return false;
            }
            else
            {
                node_type *node = cache->make(args);
                if(prev)
                {
                    items.insert_after(prev,node);
                    std::cerr << "--> insert " << args << " after " << **prev << std::endl;
                }
                else
                {
                    items.push_front(node);
                    std::cerr << "--> push front " << args << std::endl;
                }
                return true;
            }
        }

        bool contains(param_type args) const
        {
            std::cerr << "=> containing " << args << std::endl;
            node_type *prev = NULL;
            return search(args,prev);
        }

        bool remove(param_type args)
        {
            std::cerr << "=> removing " << args << std::endl;
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


        inline const list_type * operator->() const throw() { return &items; }
        inline const list_type & operator* () const throw() { return  items; }


        
    private:
        list_type items;
        zcache    cache;


        inline bool search(const_type &args, node_type * &prev) const {

            std::cerr << "--> search " << args << std::endl;
            //------------------------------------------------------------------
            //
            // dispatch search
            //
            //------------------------------------------------------------------
            assert(NULL==prev);
            switch(items.size) {
                    //----------------------------------------------------------
                    // empty list
                case  0:
                    std::cerr << "--> [empty]" << std::endl;
                    return false;
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    // check with head=tail
                case  1:
                    return search_first(args,prev);
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
                    std::cerr << "--> " << args << " < " << **items.head << " @first" << std::endl;
                    break;
                case __zero__:
                    std::cerr << "--> " << args << " = " << **items.head << " @first" << std::endl;
                    prev = items.head;
                    return true;
                case positive:
                    std::cerr << "--> " << args << " > " << **items.head << " @first" << std::endl;
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
                    std::cerr << "--> " << args << " < " << **lower << " @head" << std::endl;
                    return false;

                case __zero__:
                    std::cerr << "--> " << args << " = " << **lower << " @head" << std::endl;
                    prev = lower;
                    return true;

                case positive:
                    std::cerr << "--> " << args << " > " << **lower << " @head" << std::endl;
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
                    std::cerr << "--> " << args << " < " << **upper << " @tail" << std::endl;
                    break;

                case __zero__:
                    std::cerr << "--> " << args << " = " << **upper << " @tail" << std::endl;
                    prev = lower;
                    return true;

                case positive:
                    std::cerr << "--> " << args << " > " << **upper << " @tail" << std::endl;
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
                std::cerr << "--> " << **lower << " < " << args << " < " << **forward << " within #" << items.size << std::endl;
                prev = lower;
                return false;
            }

            switch( __sign::of(args,**forward) )
            {
                case __zero__:
                    std::cerr << "--> found " << args << " within #" << items.size << std::endl;
                    prev = forward;
                    return true; // found at forward

                case negative:
                    prev = lower;
                    std::cerr << "--> " << **lower << " < " << args << " < " << **forward << " within #" << items.size << std::endl;
                    return false; // between lower and forward

                case positive:
                    std::cerr << "--> " << args << " > " << **forward << " : move up" << std::endl;
                    lower = forward; break;
                    // move
            }

            goto STEP_FORWARD;
        }


    };

}

#endif

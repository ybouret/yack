//! \file


#ifndef YACK_SORTED_LIST_INCLUDED
#define YACK_SORTED_LIST_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/data/pool.hpp"
#include "yack/type/args.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"
#include "yack/object.hpp"
#include "yack/container.hpp"

#include <iostream>

namespace yack
{
    
    namespace low_level
    {
        extern const char sorted_list_name[]; //!< "sortes list"
    }
    
    //__________________________________________________________________________
    //
    //
    //! list of ordered data, using default '<' operator
    //
    //__________________________________________________________________________
    template <typename T>
    class sorted_list : public container
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases
        
        
        //______________________________________________________________________
        //
        //! dedicated node type for CONST data
        //______________________________________________________________________
        class node_type
        {
        public:
            //! setup from data
            inline  node_type(const_type &args) : next(0), prev(0), data(args) { }
            //! copy
            inline  node_type(const node_type &other) : next(0), prev(0), data(other.data) {}
            //! cleanup
            inline ~node_type() noexcept {}
            
            //! access
            inline const_type & operator*() const noexcept { return data; }
            
            node_type *next; //!< for list/pool
            node_type *prev; //!< for list
            
            
        private:
            const_type data;
            YACK_DISABLE_ASSIGN(node_type);
        };
        
        typedef list_of<node_type> list_type; //!< alias
        typedef pool_of<node_type> pool_type; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! setup empty
        inline explicit sorted_list() noexcept : active(), zombie() {}
        
        //! setup with capacity
        inline explicit sorted_list(const size_t n) noexcept : active(), zombie() { reserve(n); }
        
        //! cleanup
        inline virtual ~sorted_list() noexcept { release_all(); }
        
        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual const char * category() const noexcept
        {
            return low_level::sorted_list_name;
        } //!< get its name
        
        inline virtual size_t size()      const noexcept { return active.size; }             //!< active.size
        inline virtual size_t capacity()  const noexcept { return active.size+zombie.size; } //!< active.size+zombie.size
        inline virtual size_t available() const noexcept { return zombie.size; }             //!< zombie.size
        inline virtual void   free()            noexcept { zombify(); }                      //!< empty active
        inline virtual void   release()         noexcept { release_all(); }                  //!< release all
        inline virtual void   reserve(size_t n)
        {
            while(n-- > 0) zombie.store( zacquire() );
        } //!< store z-nodes
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! default insert
        inline void insert(param_type args)
        {
            node_type *node = create(args);
            if(active.size<=0)
            {
                active.push_back(node);
            }
            else
            {
                if(  **node < **active.head )
                {
                    active.push_front(node);
                }
                else
                {
                    if(  **active.tail < **node )
                    {
                        active.push_back(node);
                    }
                    else
                    {
                        assert(active.size>=1);
                        node_type *prev = active.push_back(node)->prev;
                        while(prev && **node < **prev)
                        {
                            node = active.towards_front(node);
                            prev = node->prev;
                        }
                    }
                }
                
            }
        }
        
        //! insert multiple times
        inline void insert(param_type args, size_t n)
        {
            if(n<=0)
                return;
            else
            {
                node_type *node = create(args);
                if(active.size<=0)
                {
                    active.push_back(node);
                    while(--n>0) active.push_back( create(args) );
                }
                else
                {
                    if(  **node < **active.head )
                    {
                        active.push_front(node);
                        while(--n>0) active.push_front( create(args) );
                    }
                    else
                    {
                        if(  **active.tail < **node )
                        {
                            active.push_back(node);
                            while(--n>0) active.push_back( create(args) );
                        }
                        else
                        {
                            assert(active.size>=1);
                            node_type *prev = active.push_back(node)->prev;
                            while(prev && **node < **prev)
                            {
                                node = active.towards_front(node);
                                prev = node->prev;
                            }
                            while(--n>0)
                                active.insert_after(node,create(args));
                        }
                    }
                    
                }
            }
        }
        
        
        //! head node, the 'smallest'
        inline const node_type *head() const noexcept { return active.head; }
        
        //! tail node, the 'greatest'
        inline const node_type *tail() const noexcept { return active.tail; }
        
        //! 'lower' value
        inline const_type & front() const noexcept { assert(active.head); return **active.head; }
        
        //! 'upper' value
        inline const_type & back() const noexcept { assert(active.tail); return **active.tail; }
        
        //! remove head
        inline void pop_front() noexcept { assert(active.head); zombify( active.pop_front() ); }
        
        //! remove tail
        inline void pop_back() noexcept  { assert(active.tail); zombify( active.pop_back() ); }
        
        //! remove head, return its copy
        inline const_type pull_front() {
            assert(active.head);
            const_type ans( **active.head );
            pop_front();
            return ans;
        }
        
        //! remove tail, return its copy
        inline const_type pull_back() {
            assert(active.tail);
            const_type ans( **active.tail );
            pop_back();
            return ans;
        }
        
        
        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const sorted_list &L)
        {
            os << '{';
            const node_type *node = L.active.head;
            if(node)
            {
                os << **node;
                for(node=node->next;node;node=node->next)
                {
                    os << ';' << **node;
                }
            }
            
            return os << '}';
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(sorted_list);
        list_type active;
        pool_type zombie;
        
        inline node_type *create(const_type &args)
        {
            node_type *node = zombie.size ? zombie.query() : zacquire();
            try {
                return new (node) node_type(args);
            }
            catch(...)
            {
                zombie.store( out_of_reach::naught(node) );
                throw;
            }
        }
        
        inline void zombify(node_type *node) noexcept
        {
            assert(NULL!=node);
            zombie.store( out_of_reach::naught( destructed(node) ) );
        }
        
        inline void zombify() noexcept
        {
            while(active.size>0) zombify(active.pop_back());
            
        }
        
        inline void release_zombie() noexcept
        {
            while(zombie.size>0) zrelease(zombie.query());
        }
        
        inline void release_active() noexcept
        {
            while(active.size>0) zrelease( destructed(active.pop_back()) );
        }
        
        inline void release_all() noexcept
        {
            release_zombie();
            release_active();
        }
        
        
        static node_type *zacquire() {
            return object::zacquire<node_type>();
        }
        
        
        static void zrelease(node_type *node) noexcept {
            object::zrelease(node);
        }
        
        
    };
    
}

#endif


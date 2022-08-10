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
            inline ~node_type() throw() {}

            //! access
            inline const_type & operator*() const throw() { return data; }

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
        inline explicit sorted_list() throw() : active(), zombie() {}

        //! setup with capacity
        inline explicit sorted_list(const size_t n) throw() : active(), zombie() { reserve(n); }

        //! cleanup
        inline virtual ~sorted_list() throw() { release_all(); }

        //______________________________________________________________________
        //
        // container interfacer
        //______________________________________________________________________
        inline virtual const char * category() const throw()
        {
            return low_level::sorted_list_name;
        } //!< get its name

        inline virtual size_t size()     const throw()  { return active.size; }             //!< active.size
        inline virtual size_t capacity() const throw()  { return active.size+zombie.size; } //!< active.size+zombie.size
        inline virtual size_t available() const throw() { return zombie.size; }             //!< zombie.size
        inline virtual void   free()            throw() { zombify(); }                      //!< empty active
        inline virtual void   release()         throw() { release_all(); }                  //!< release all
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

        //! head node, the 'smallest'
        inline const node_type *head() const throw() { return active.head; }

        //! tail node, the 'greatest'
        inline const node_type *tail() const throw() { return active.tail; }

        //! 'lower' value
        inline const_type & front() const throw() { assert(active.head); return **active.head; }

        //! 'upper' value
        inline const_type & back() const throw() { assert(active.tail); return **active.tail; }

        //! remove head
        inline void pop_front() throw() { assert(active.head); zombify( active.pop_front() ); }

        //! remove tail
        inline void pop_back() throw()  { assert(active.tail); zombify( active.pop_back() ); }

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

        inline void zombify(node_type *node) throw()
        {
            zombie.store( out_of_reach::naught( destructed(node) ) );
        }

        inline void zombify() throw()
        {
            while(active.size) zombify(active.pop_back());

        }
        
        inline void release_zombie() throw()
        {
            while(zombie.size) zrelease(zombie.query());
        }
        
        inline void release_active() throw()
        {
            while(active.size) zrelease( destructed(active.pop_back()) );
        }
        
        inline void release_all() throw()
        {
            release_zombie();
            release_active();
        }
        
        
        static node_type *zacquire() {
            return object::zacquire<node_type>();
        }
        
        
        static void zrelease(node_type *node) throw() {
            object::zrelease(node);
        }

        
    };
    
}

#endif


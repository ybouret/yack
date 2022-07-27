//! \file


#ifndef YACK_ORDERED_LIST_INCLUDED
#define YACK_ORDERED_LIST_INCLUDED

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
        extern const char ordered_list_name[]; //!< "ordered list"
    }

    //__________________________________________________________________________
    //
    //
    //! list of ordered data, using default '<' operator
    //
    //__________________________________________________________________________
    template <typename T>
    class ordered_list : public container
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
        inline explicit ordered_list() throw() : active(), zombie() {}

        //! setup with capacity
        inline explicit ordered_list(const size_t n) throw() : active(), zombie() { reserve(n); }

        //! cleanup
        inline virtual ~ordered_list() throw() { release_all(); }

        //______________________________________________________________________
        //
        // container interfacer
        //______________________________________________________________________
        inline virtual const char * category() const throw()
        {
            return low_level::ordered_list_name;
        }
        inline virtual size_t size()     const throw()  { return active.size; }
        inline virtual size_t capacity() const throw()  { return active.size+zombie.size; }
        inline virtual size_t available() const throw() { return zombie.size; }
        inline virtual void   free()            throw() { zombify(); }
        inline virtual void   release()         throw() { release_all(); }
        inline virtual void   reserve(size_t n)
        {
            while(n-- > 0) zombie.store( zacquire() );
        }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! default insert
        inline void insert(param_type args)
        {
            node_type *node = new_node(args);
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
                        assert(active.size>=2);
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

        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const ordered_list &L)
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
        YACK_DISABLE_COPY_AND_ASSIGN(ordered_list);
        list_type active;
        pool_type zombie;
        
        inline node_type *new_node(const_type &args)
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
        
        inline void zombify() throw()
        {
            while(active.size) zombie.store( out_of_reach::naught( destructed(active.pop_back()) ) );

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


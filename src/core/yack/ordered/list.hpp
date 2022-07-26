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

namespace yack
{
    
    namespace low_level
    {
        extern const char ordered_list_name[];
    }
    
    template <typename T>
    class ordered_list : public container
    {
    public:
        YACK_DECL_ARGS(T,type);
        class node_type
        {
        public:
            inline  node_type(const_type &args) : next(0), prev(0), data(args) { }
            inline ~node_type() throw() {}
            inline  node_type(const node_type &other) : next(0), prev(0), data(other.data) {}
            node_type *next;
            node_type *prev;
        private:
            T data;
            YACK_DISABLE_ASSIGN(node_type);
        };
        
        typedef list_of<node_type> list_type;
        typedef pool_of<node_type> pool_type;
        
        inline explicit ordered_list() throw() : active(), zombie() {}
        inline virtual ~ordered_list() throw()
        {
            release_all();
        }
        
        // container
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
        
        // methods
        
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ordered_list);
        list_type active;
        pool_type zombie;
        
        inline node_type new_node(const_type &args)
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


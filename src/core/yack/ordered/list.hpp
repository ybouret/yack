//! \file


#ifndef YACK_ORDERED_LIST_INCLUDED
#define YACK_ORDERED_LIST_INCLUDED

#include "yack/data/list.hpp"
#include "yack/data/pool.hpp"
#include "yack/type/args.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"
#include "yack/object.hpp"

namespace yack
{
    
    template <typename T>
    class ordered_list
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
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ordered_list);
        list_type active;
        pool_type zombie;
        
        
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


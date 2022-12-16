#if 0
//! \file

#ifndef YACK_DATA_SMALL_REPO_INCLUDED
#define YACK_DATA_SMALL_REPO_INCLUDED 1

#include "yack/data/small/list.hpp"
#include "yack/data/pool.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"
#include "yack/type/transmogrify.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! acting as a cache
    //
    //__________________________________________________________________________
    template <typename T>
    class small_repo
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);               //!< aliases
        typedef small_node<T>      node_type; //!< alias
        typedef klist<T,node_type> list_type; //!< alias
        typedef pool_of<node_type> pool_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit small_repo() throw() :  list(), pool() {} //!< setup
        inline virtual ~small_repo() throw() { release(); }       //!< cleanup

        //! copy list
        inline small_repo(const small_repo &other) : list(other.list), pool() {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline list_type *       operator->()       throw() { return &list; } //!< access
        inline const list_type * operator->() const throw() { return &list; } //!< access
        inline list_type &       operator *()       throw() { return  list; } //!< access
        inline const list_type & operator *() const throw() { return  list; } //!< access

        //! push back by copy, return new object reference
        inline type & push_back(param_type args)
        {
            return **list.push_back( create(args) );
        }

        //! push front by copy, return new object reference
        inline type & push_front(param_type args)
        {
            return **list.push_front( create(args) );
        }

        //! remove tail
        inline void pop_back()
        {
            assert(list.size);
            zstore(list.pop_back());
        }

        //! remove head
        inline void pop_front()
        {
            assert(list.size);
            zstore(list.pop_front());
        }

        //! store an alive node
        inline void zstore(node_type *node) throw()
        {
            assert(node);
            pool.store( out_of_reach::naught( destructed(node) ) );
        }

        //! free content, keep cache
        inline void free() throw()
        {
            while(list.size) zstore(list.pop_back());
        }

        //! release all
        inline void release() throw()
        {
            while(list.size)  delete list.pop_back();
            trim();
        }

        //! clean cache
        inline void trim() throw()
        {
            while(pool.size) object::zrelease(pool.query());
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        list_type list; //!< list of objects
        pool_type pool; //!< pool of zombies

    private:
        YACK_DISABLE_ASSIGN(small_repo);

        template <typename U>
        inline node_type *create(const U &args)
        {
            if(pool.size)
            {
                node_type *node = pool.query();
                try {
                    return new (node) node_type(args);
                }
                catch(...)
                {
                    pool.store( out_of_reach::naught(node) );
                    throw;
                }
            }
            else
            {
                return new node_type(args);
            }
        }





    };

}

#endif
#endif

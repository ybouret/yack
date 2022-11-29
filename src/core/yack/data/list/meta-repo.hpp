
//! \file

#ifndef YACK_DATA_META_REPO_INCLUDED
#define YACK_DATA_META_REPO_INCLUDED 1

#include "yack/data/pool/meta.hpp"
#include "yack/data/list/meta.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! local repository of meta nodes, to be used as cache
    //
    //__________________________________________________________________________
    template <typename T>
    class meta_repo
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef meta_node<T> node_type; //!< alias
        typedef meta_pool<T> pool_type; //!< alias
        typedef meta_list<T> list_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit meta_repo() throw() : list(), pool() {}                     //!< setup
        inline virtual ~meta_repo() throw() {}                                      //!< cleanup
        inline explicit meta_repo(const size_t n) : list(), pool() { reserve(n); }  //!< setup with empty nodes

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline list_type *       operator->()       throw() { return &list; } //!< access
        inline const list_type * operator->() const throw() { return &list; } //!< access

        //! reserve memory
        inline void reserve(size_t n) {
            while(n-- > 0) pool.store( new node_type(NULL) );
        }

        //! free list into pool
        inline void free() throw()
        {
            while(list.size) zstore(list.pop_back());
        }

        //! create new node with on-the-fly cached/created memory
        inline void push_back(T &obj)
        {
            if(pool.size)
                list.push_back( pool.query() )->link(&obj);
            else
                list.push_back( new node_type(&obj) );
        }

        //! create new node with on-the-fly cached/created memory
        inline void push_front(T &obj)
        {
            if(pool.size)
                list.push_front( pool.query() )->link(&obj);
            else
                list.push_front( new node_type(&obj) );
        }

        //! remove last node into pool
        inline void pop_back() throw()
        {
            zstore(list.pop_back());
        }

        //! remove first node into pool
        inline void pop_front() throw()
        {
            zstore(list.pop_front());
        }


        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        list_type list; //!< active nodes
        pool_type pool; //!< zombie nodes


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(meta_repo);
        inline void zstore(node_type *node) throw()
        {
            assert(node);
            pool.store(node)->link(NULL);
        }
    };

}

#endif

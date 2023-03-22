//! \file

#ifndef YACK_DINKY_ROOT_INCLUDED
#define YACK_DINKY_ROOT_INCLUDED 1

#include "yack/type/transmogrify.hpp"
#include "yack/data/list.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! common API for lists
    //
    //__________________________________________________________________________
    template <class NODE, class ZPROXY>
    class dinky_root : public list_of<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef NODE          node_type; //!< alias
        typedef list_of<NODE> list_type; //!< alias
        
        using list_type::push_back;
        using list_type::push_front;
        using list_type::pop;
        using list_type::pop_back;
        using list_type::pop_front;
        using list_type::size;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! setup for none/solo cache
        inline explicit dinky_root() noexcept : list_type(), cache() {}
        
        //! setup for coop cache
        inline explicit dinky_root(const ZPROXY &user) noexcept : list_type(), cache(user) {}
        
        //! cleanup
        inline virtual ~dinky_root() noexcept { cache->devour(*this); }
        
        //! copy
        inline dinky_root(const dinky_root &other) :
        list_type(), cache(other.cache)
        {
            replicate(*this,other);
        }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! push back a default node
        inline node_type *annex()  { return push_back( cache->create() ); }

        //! push front a default node
        inline node_type *shove() { return push_front( cache->create() ); }

        //! push back with one argument
        template <typename U>
        inline node_type * annex(const U &u) { return push_back(  cache->create(u) ); }
        
        //! push front with one argument
        template <typename U>
        inline node_type * shove(const U &u) { return push_front( cache->create(u) ); }


        //! push back with two arguments
        template <typename U, typename V>
        inline node_type * annex(const U &u, const V &v) { return push_back(  cache->create(u,v) ); }


        //! push front with two arguments
        template <typename U, typename V>
        inline node_type * shove(const U &u, const V &v) { return push_front(  cache->create(u,v) ); }


        //! push back with three arguments
        template <typename U, typename V, typename W>
        inline node_type * annex(const U &u, const V &v, const W &w) { return push_back(  cache->create(u,v,w) ); }

        //! push front with three arguments
        template <typename U, typename V, typename W>
        inline node_type * shove(const U &u, const V &v, const W &w) { return push_front(  cache->create(u,v,w) ); }



        //! remove node
        inline void cut(NODE *node) noexcept {
            assert(node);
            cache->ingest( pop(node) );
            //cache->zstore( cache->turn( pop(node) ) );
        }
        
        //! remove back
        inline void cut_tail() noexcept
        {
            assert(size>=0);
            cache->ingest( pop_back() );
            //cache->zstore( cache->turn( pop_back() ) );
        }

        //! remove back n
        inline void cut_tail(size_t n) noexcept
        {
            assert(size>=n);
            while(n-- > 0) cut_tail();
        }
        
        //! remove front
        inline void cut_head() noexcept
        {
            assert(size);
            cache->ingest( pop_front() );
            //cache->zstore( cache->turn( pop_front() ) );
        }

        //! remove front n
        inline void cut_head(size_t n) noexcept
        {
            assert(size>=n);
            while(n-- > 0) cut_head();
        }

        //! insert new node after mine
        template <typename U>
        inline void after(node_type *mine, const U &u)
        {
            this->insert_after(mine, cache->create(u) );
        }
        
        //! insert new node ahead of mine
        template <typename U>
        inline void ahead(node_type *mine, const U &u)
        {
            this->insert_before(mine, cache->create(u) );
        }

        //! merge back copy
        inline void merge_back_copy(const dinky_root &source)
        {
            list_of<NODE> target;
            replicate(target,source);
            this->merge_back(target);
        }

        //! merge front copy
        inline void merge_front_copy(const dinky_root &source)
        {
            list_of<NODE> target;
            replicate(target,source);
            this->merge_front(target);
        }


        //! syntax helper
        inline dinky_root & operator+=(const dinky_root &source)
        {
            merge_back_copy(source);
            return *this;
        }
        
        //! clear alive NODEs
        inline void   clear() noexcept { cache->devour(*this); }
        
        //! avalailable zombies
        inline size_t ready() const noexcept { return coerce(cache)->stowage(); }
        
        
        
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        ZPROXY cache; //!< cache with zproxy API
        
    private:
        YACK_DISABLE_ASSIGN(dinky_root);
        inline void replicate(list_of<NODE>       &target,
                              const list_of<NODE> &source)
        {
            assert(0==target.size);
            try
            {
                for(const NODE *node=source.head;node;node=node->next)
                    target.push_back( cache->create( **node) );
            } catch(...) { cache->devour(target); throw; }
        }
    
    };
}


#endif


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
        inline explicit dinky_root() throw() : list_type(), cache() {}
        
        //! setup for coop cache
        inline explicit dinky_root(const ZPROXY &user) throw() : list_type(), cache(user) {}
        
        //! cleanup
        inline virtual ~dinky_root() throw() { cache->devour(*this); }
        
        //! copy
        inline dinky_root(dinky_root &other) :
        list_type(), cache(other.cache)
        {
            replicate(*this,other);
        }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
       
        //! push back
        template <typename U>
        inline void annex(const U &u) { push_back(  cache->create(u) ); }
        
        //! push front
        template <typename U>
        inline void shove(const U &u) { push_front( cache->create(u) ); }
        
        //! remove node
        inline void cut(NODE *node) throw() {
            assert(node);
            cache->zstore( cache->turn( pop(node) ) );
        }
        
        //! remove back
        inline void cut_tail() throw()
        {
            assert(size);
            cache->zstore( cache->turn( pop_back() ) );
        }
        
        //! remove front
        inline void cut_head() throw()
        {
            assert(size);
            cache->zstore( cache->turn( pop_front() ) );
        }
        
        template <typename U>
        inline void after(node_type *mine, const U &u)
        {
            this->insert_after(mine, cache->create(u) );
        }
        
        template <typename U>
        inline void ahead(node_type *mine, const U &u)
        {
            this->insert_before(mine, cache->create(u) );
        }
        
        //! merge back copy
        inline dinky_root & operator+=(const dinky_root &source)
        {
            list_of<NODE> target;
            replicate(target,source);
            this->merge_back(target);
            return *this;
        }
        
        //! clear alive NODEs
        inline void clear() throw()
        {
            cache->devour(*this);
        }
        
        //! avalailable zombies
        inline size_t ready() const throw()
        {
            return coerce(cache)->stowage();
        }
        
        
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


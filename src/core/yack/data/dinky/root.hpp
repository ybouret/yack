
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
        typedef NODE          node_type;
        typedef list_of<NODE> list_type;
        
        using list_type::push_back;
        using list_type::push_front;

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
        template <typename U> void annex(const U &u) { push_back(  cache->create(u) ); }
        template <typename U> void shove(const U &u) { push_front( cache->create(u) ); }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        ZPROXY cache;
        
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


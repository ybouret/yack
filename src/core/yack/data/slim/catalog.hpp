//! \file

#ifndef YACK_DATA_SLIM_CATALOG_INCLUDED
#define YACK_DATA_SLIM_CATALOG_INCLUDED 1

#include "yack/data/slim/zpool.hpp"
#include "yack/data/slim/warden.hpp"
#include "yack/data/list.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! manage list and single/shared cache
    //
    //__________________________________________________________________________
    template <typename NODE, typename ZPOOL>
    class slim_catalog : public  list_of<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef NODE                        node_type; //!< alias
        typedef list_of<NODE>               base_type; //!< alias
        typedef ZPOOL                       pool_type; //!< alias
        typedef slim_catalog<NODE,ZPOOL>    this_type; //!< alias
        typedef slim_warden<NODE,this_type> warden_type; //!< alias

        //______________________________________________________________________
        //
        // using...
        //______________________________________________________________________
        using base_type::push_back;
        using base_type::push_front;
        using base_type::merge_back;
        using base_type::merge_front;
        using base_type::pop_back;
        using base_type::pop_front;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~slim_catalog() throw() { cache->zfinal(*this); assert(0==this->size);}

        //! setup empty: OK with slim_hook
        inline explicit slim_catalog() throw() : base_type(), cache() {}

        //! setup with shared: OK with slim_bank::pointer
        inline explicit slim_catalog(const ZPOOL &shared) throw() :
        base_type(), cache(shared) {}

        //! copy by duplication
        inline slim_catalog(const slim_catalog &other) :
        base_type(), cache(other.cache) { duplicate(other); }

        
        //______________________________________________________________________
        //
        // management methods
        //______________________________________________________________________
       
        
        template <typename U> inline
        node_type *annex(const U &u) { return push_back( create(u) ); }
        
        template <typename U> inline
        node_type *shove(const U &u) { return push_front( create(u) ); }
        
        
        

        //! merge back a copy of a (possibly alien) list
        template <typename KNOT> inline
        slim_catalog & operator += (const list_of<KNOT> &other) {
            list_of<NODE> l; merge_back(populate(l,other));
            return *this;
        }

        inline void zfront() throw() { assert(this->size>0); cache->zstore( pop_front() ); } //!< zombify head
        inline void zback()  throw() { assert(this->size>0); cache->zstore( pop_back()  ); } //!< zombify tail
        inline void erase()  throw() { cache->zstore( *this ); assert(0==this->size); }      //!< zombify content
        inline void prune()  throw() { cache->zfinal( *this ); assert(0==this->size); }      //!< depending on cache

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        pool_type           cache; //!< cache of zombie nodes
        
    protected:
        
        //! instantiate a new node with compatible argument
        template <typename U> inline
        NODE *create(const U &u)
        {
            NODE *node =  cache->zquery();
            try { return new(node) NODE(u,transmogrify); }
            catch(...) { cache->zstore(node); throw;   }
        }

    private:
        YACK_DISABLE_ASSIGN(slim_catalog);
        
       
        inline void duplicate(const list_of<NODE> &other)
        {
            try
            {
                for(const node_type *node=other.head;node;node=node->next)
                    push_back( create(**node) );
            }
            catch(...) { erase(); throw; }
        }

        template <typename KNOT> inline
        list_of<NODE> &populate(list_of<NODE> &l, const list_of<KNOT> &other)
        {
            try
            {
                for(const KNOT *knot=other.head;knot;knot=knot->next)
                    l.push_back( create(**knot) );
                return l;
            }
            catch(...) { cache->zstore(l); throw; }
        }



    };

}

#endif

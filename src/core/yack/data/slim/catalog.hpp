//! \file

#ifndef YACK_DATA_SLIM_CATALOG_INCLUDED
#define YACK_DATA_SLIM_CATALOG_INCLUDED 1

#include "yack/data/slim/zpool.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! handle list and single/shared deposit
    //
    //__________________________________________________________________________
    template <typename NODE, typename ZPOOL>
    class slim_catalog : public  cxx_list_of<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef NODE node_type;              //!< alias
        typedef cxx_list_of<NODE> base_type; //!< alias
        using base_type::push_back;
        using base_type::push_front;
        using base_type::merge_back;
        using base_type::merge_front;
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~slim_catalog() throw() { repo->zfinal(*this); }

        //! setup empty: OK with slim_hook
        inline explicit slim_catalog() throw() : base_type(), repo() {}

        //! setup with shared: OK with slim_bank::pointer
        inline explicit slim_catalog(const ZPOOL &shared) throw() :
        base_type(), repo(shared) {}

        //! copy by duplication
        inline slim_catalog(const slim_catalog &other) :
        base_type(),
        repo(other.repo)
        {
            duplicate(other);
        }

        
        //______________________________________________________________________
        //
        // management methods
        //______________________________________________________________________

        
        //! push_back wrapper for an element
        template <typename U> inline
        slim_catalog & operator<<(const U &u) {
            push_back( create(u) );
            return *this;
        }


        //! push_front wrapper for an element
        template <typename U> inline
        slim_catalog & operator>>(const U &u) {
            push_front( create(u) );
            return *this;
        }

        //! merge back a copy of a (possibly alien) list
        template <typename KNOT> inline
        slim_catalog & operator += (const list_of<KNOT> &other) {
            list_of<NODE> l; merge_back(populate(l,other));
            return *this;
        }

        
        //! zombify content
        inline void free() throw() { repo->zstore(*this); }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        ZPOOL              repo; //!< repository of zombie nodes
        
    protected:
        
        //! instantiate a new node with compatible argument
        template <typename U> inline
        NODE *create(const U &u)
        {
            NODE *node =  repo->zquery();
            try { return new(node) NODE(u,transmogrify); }
            catch(...) { repo->zstore(node); throw;   }
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
            catch(...) { free(); throw; }
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
            catch(...) { repo->zstore(l); throw; }
        }



    };

}

#endif

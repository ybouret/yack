
//! \file

#ifndef YACK_DATA_SLIM_MANIFEST_INCLUDED
#define YACK_DATA_SLIM_MANIFEST_INCLUDED 1

#include "yack/data/slim/zpool.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! handle listing and single/shared deposit
    //
    //__________________________________________________________________________
    template <typename NODE, typename ZPOOL>
    class slim_manifest
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef NODE node_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~slim_manifest() throw() { deposit->zfinal(listing); }

        //! setup empty: OK with slim_hook
        inline explicit slim_manifest() throw() : listing(), deposit() {}

        //! setup with shared: OK with slim_bank::pointer
        inline explicit slim_manifest(const ZPOOL &shared) throw() :
        listing(), deposit(shared) {}

        //! copy by duplication
        inline slim_manifest(const slim_manifest &other) :
        listing(),
        deposit(other.deposit)
        {
            duplicate(other.listing);
        }

        //______________________________________________________________________
        //
        // access methods
        //______________________________________________________________________
        inline const list_of<NODE> & operator*()  const throw() { return  listing; } //!< access
        inline const list_of<NODE> * operator->() const throw() { return &listing; } //!< access

        //______________________________________________________________________
        //
        // management methods
        //______________________________________________________________________

        //! push_back wrapper for an element
        template <typename U> inline
        slim_manifest & operator<<(const U &u) {
            listing.push_back( create(u) );
            return *this;
        }


        //! push_front wrapper for an element
        template <typename U> inline
        slim_manifest & operator>>(const U &u) {
            listing.push_front( create(u) );
            return *this;
        }

        //! merge back a copy of a (possibly alien) list
        template <typename KNOT> inline
        slim_manifest & operator<<(const list_of<KNOT> &other)
        {
            list_of<NODE> l;
            listing.merge_back(populate(l,other));
            return *this;
        }

        //! merge front a copy of a (possibly alien) list
        template <typename KNOT> inline
        slim_manifest & operator>>(const list_of<KNOT> &other)
        {
            list_of<NODE> l;
            listing.merge_front(populate(l,other));
            return *this;
        }


        //! zombify listing content
        inline void free() throw() { deposit->zstore(listing); }


    private:
        YACK_DISABLE_ASSIGN(slim_manifest);
        cxx_list_of<NODE>  listing;

    public:
        ZPOOL              deposit; //!< deposit of zombie nodes

    private:
        template <typename U> inline
        NODE *create(const U &u)
        {
            NODE *node =  deposit->zquery();
            try { return new(node) NODE(u,transmogrify); }
            catch(...) { deposit->zstore(node); throw;   }
        }

        inline void duplicate(const list_of<NODE> &other)
        {
            try
            {
                for(const node_type *node=other.head;node;node=node->next)
                    listing.push_back( create(**node) );
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
            catch(...) { deposit->zstore(l); throw; }
        }



    };

}

#endif

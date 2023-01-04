//! \file

#ifndef YACK_SLIM_WARDEN_INCLUDED
#define YACK_SLIM_WARDEN_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/data/slim/ptr-nil.hpp"
#include <iostream>

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! replacement for auto_ptr when using managed lists
    //
    //__________________________________________________________________________
    template <typename NODE, typename CATALOG>
    class slim_warden
    {
    public:
        //! initialize with node and its source
        inline  slim_warden(NODE *knot, CATALOG &source) throw() : node(knot), pool(source.cache) {}

        //! auto-clean
        inline ~slim_warden() throw() { if(node) { pool->zstore(node); node=0; } }


        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline  bool  is_empty() const throw() { return 0==node; } //!< test if is empty
        inline  bool  is_valid() const throw() { return 0!=node; } //!< test if is valid
        inline  NODE *yield()          throw() { NODE *knot = node; node=NULL; return knot; } //!< yield ownership


        inline NODE &       operator*()        throw() { assert(node); return *node; } //!< access
        inline const NODE & operator*()  const throw() { assert(node); return *node; } //!< access, const
        inline NODE *       operator->()       throw() { assert(node); return  node; } //!< access
        inline const NODE * operator->() const throw() { assert(node); return  node; } //!< access

        inline friend std::ostream & operator<<(std::ostream &os, const slim_warden &self)
        {
            return self.node ? (os << *self.node) : (os << kernel::slim_ptr_nil);
        }


    private:
        NODE                        *node;
        typename CATALOG::pool_type &pool;
        YACK_DISABLE_COPY_AND_ASSIGN(slim_warden);
    };



}

#endif


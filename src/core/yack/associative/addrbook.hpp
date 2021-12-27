//! \file

#ifndef YACK_ASSOCIATIVE_ADDRBOOK_INCLUDED
#define YACK_ASSOCIATIVE_ADDRBOOK_INCLUDED 1

#include "yack/data/suffix/tree.hpp"
#include "yack/container/iterator/linked.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! base type for and address book
    //
    //__________________________________________________________________________
    typedef kernel::suffix_tree<void *,uint8_t> addrbook_;

    //__________________________________________________________________________
    //
    //
    //! book of addresses
    //
    //__________________________________________________________________________
    class addrbook : public addrbook_
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        ~addrbook() throw();        //!< cleanup
        addrbook()  throw();        //!< setup
        addrbook(const addrbook &); //!< copy
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        bool              insert(const void *);               //!< insert, address is BE key
        bool              search(const void *) const throw(); //!< search if registered

        typedef iterating::linked<void *,const knot_type,iterating::forward> const_iterator;

        const_iterator begin() const throw() { return (**this).head; }
        const_iterator end()   const throw() { return NULL; }

    private:
        YACK_DISABLE_ASSIGN(addrbook);
    };
    
}

#endif


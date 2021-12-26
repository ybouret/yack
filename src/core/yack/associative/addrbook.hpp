//! \file

#ifndef YACK_ASSOCIATIVE_ADDRBOOK_INCLUDED
#define YACK_ASSOCIATIVE_ADDRBOOK_INCLUDED 1

#include "yack/data/suffix/tree.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! book of addresses
    //
    //__________________________________________________________________________
    class addrbook
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef kernel::suffix_tree<void *,uint8_t> tree_type;
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        ~addrbook() throw(); //!< cleanup
        addrbook()  throw(); //!< setup
        addrbook(const addrbook &); //!< copy
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        bool              insert(const void *);               //!< insert, BE
        bool              search(const void *) const throw(); //!< searc
        const tree_type & operator*() const throw();          //!< content
        
    private:
        YACK_DISABLE_ASSIGN(addrbook);
        tree_type db;
    };
    
}

#endif


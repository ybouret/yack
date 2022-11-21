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
        void              ensure(const void *);               //!< ensure presence
        bool              insert(const void *);               //!< insert, address is BE key
        bool              search(const void *) const throw(); //!< search if registered

        //______________________________________________________________________
        //
        // iterator
        //______________________________________________________________________

        //! alias, const only
        typedef iterating::linked<void *,const knot_type,iterating::forward> const_iterator;

        const_iterator begin() const throw() { return (**this).head; } //!< forward iterator begin
        const_iterator end()   const throw() { return NULL; }          //!< forward itetator end

        //______________________________________________________________________
        //
        //! transfer content, no cleanup of target
        //______________________________________________________________________
        template <typename T> inline
        void upload(sequence<T *> &target) const
        {
            for(const_iterator it=begin();it!=end();++it)
            {
                const T &obj = *static_cast<const T *>( *it );
                target << & coerce(obj);
            }
        }

        //______________________________________________________________________
        //
        //! retrieve exact content
        //______________________________________________________________________
        template <typename T> inline
        void retrieve(sequence<T *> &target) const
        {
            target.free();
            upload(target);
        }


        
    private:
        YACK_DISABLE_ASSIGN(addrbook);
    };
    
}

#endif



//! \file

#ifndef YACK_DINKY_ZNONE_INCLUDED
#define YACK_DINKY_ZNONE_INCLUDED 1

#include "yack/data/dinky/zcache.hpp"
#include "yack/object.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! no-cache: use directly object API
    //
    //__________________________________________________________________________
    template <typename NODE>
    class znone : public zcache<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit znone() throw() : zcache<NODE>() {} //!< setup
        inline virtual ~znone() throw() {}                  //!< cleanup
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline virtual void   release() throw()       {}            //!< do nothing
        inline virtual size_t stowage() const throw() { return 0; } //!< no reserve
        inline virtual void   reserve(size_t)         {}            //!< no reserve
        
        //______________________________________________________________________
        //
        // zcache interface
        //______________________________________________________________________
        
        //! get NODE as one object block
        inline virtual NODE * zquery()
        {
            return object::zacquire<NODE>();
        }
        
        //! release NODE as one object block
        inline virtual void   zstore(NODE *node) throw() { object::zrelease(node); }
        
        //! turn a list of alive into zombies
        inline virtual void   devour(list_of<NODE> &live) throw()
        {
            while(live.size) zstore( this->turn(live.pop_back()) );
        }
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(znone);
    };
}

#endif

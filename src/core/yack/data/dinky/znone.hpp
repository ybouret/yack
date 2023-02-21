
//! \file

#ifndef YACK_DINKY_ZNONE_INCLUDED
#define YACK_DINKY_ZNONE_INCLUDED 1

#include "yack/data/dinky/zcache.hpp"
#include "yack/data/dinky/zproxy.hpp"
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
        // types
        //______________________________________________________________________
        typedef zproxy< znone<NODE> > proxy; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit znone() noexcept : zcache<NODE>() {} //!< setup
        inline virtual ~znone() noexcept {}                  //!< cleanup
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline virtual void   release() noexcept       {}            //!< do nothing
        inline virtual size_t stowage() const noexcept { return 0; } //!< no reserve
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
        inline virtual void   zstore(NODE *node) noexcept { object::zrelease(node); }
        
        //! turn a list of alive into zombies
        inline virtual void   devour(list_of<NODE> &live) noexcept
        {
            while(live.size) zstore( this->turn(live.pop_back()) );
        }
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(znone);
    };
}

#endif


//! \file

#ifndef YACK_DATA_ZLINKED_INCLUDED
#define YACK_DATA_ZLINKED_INCLUDED 1


#include "yack/object.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! zombie linked interfaxe
    //
    //__________________________________________________________________________
    template <typename NODE>
    class zlinked : public releasable
    {
    public:
        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________
        virtual NODE  *zquery() = 0;                //!< query a zombie node
        virtual void   zstore(NODE *) throw() = 0;  //!< store a zombie node

        //______________________________________________________________________
        //
        // non virtual interface
        //______________________________________________________________________

        //! reserve nodes
        inline void reserve(size_t n) { while(n-- >0 ) zstore( zcreate() ); }

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________
        inline static NODE *zcreate()                   { return object::zacquire<NODE>(); } //!< small object
        inline static void  zdelete(NODE *node) throw() { object::zrelease(node);          } //!< small object

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~zlinked() throw() {}
    protected:
        inline explicit zlinked() throw() : releasable() {}


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(zlinked);
    };
}

#endif

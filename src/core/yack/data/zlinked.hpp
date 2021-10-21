
//! \file

#ifndef YACK_DATA_ZLINKED_INCLUDED
#define YACK_DATA_ZLINKED_INCLUDED 1


#include "yack/object.hpp"
#include "yack/container/releasable.hpp"
#include "yack/type/out-of-reach.hpp"

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
        virtual size_t size() const throw()   = 0;  //!< available

        //______________________________________________________________________
        //
        // non virtual interface
        //______________________________________________________________________
        inline void zstore(NODE *node) throw() {
            assert(NULL!=node);
            _zstore( static_cast<NODE *>(out_of_reach::zset(node,sizeof(NODE)) ) );
        }

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
        virtual void   _zstore(NODE *) throw() = 0;  //!< store a zombie node
    };
}

#endif

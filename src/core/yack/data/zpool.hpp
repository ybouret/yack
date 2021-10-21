//! \file

#ifndef YACK_DATA_ZPOOL_INCLUDED
#define YACK_DATA_ZPOOL_INCLUDED 1

#include "yack/data/pool.hpp"
#include "yack/data/zlinked.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! pool with zombie NODEs
    //
    //__________________________________________________________________________
    template <typename NODE>
    class zpool_of :   public zlinked<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit zpool_of() throw() : zlinked<NODE>(), impl()  {}
        inline virtual ~zpool_of() throw() { clear(); }

        //______________________________________________________________________
        //
        // zlinked interface
        //______________________________________________________________________
        inline virtual NODE  *zquery() { return impl.size ? impl.query() : this->zcreate(); }
        inline virtual size_t size() const throw() { return impl.size; }

        //______________________________________________________________________
        //
        // release interface
        //______________________________________________________________________
        virtual void release() throw() { clear(); }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(zpool_of);
        pool_of<NODE> impl;
        inline virtual void  _zstore(NODE *node) throw() { assert(node); impl.store(node); }

        inline void clear() throw()
        {
            while(impl.size) this->zdelete(impl.query());
        }
    };

}

#endif
